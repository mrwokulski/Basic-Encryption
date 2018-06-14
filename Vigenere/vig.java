import java.io.*;
import java.lang.*;
import java.util.*;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;

public class vig{

	private static final double[] LETTER_FREQUENCIES =
					{0.08167, 0.01492, 0.02782, 0.04253, 0.12702,
									0.02228, 0.02015, 0.06094, 0.06966, 0.00153, 0.00772,
									0.04025, 0.02406, 0.06749, 0.07507, 0.01929, 0.00095,
									0.05987, 0.06327, 0.09056, 0.02758, 0.00978, 0.02360,
									0.00150, 0.01974, 0.00074};

	public static void prep(){

		Path orig = Paths.get("orig.txt");
		Charset charset = Charset.forName("US-ASCII");

		try {
			List<String> lines = Files.readAllLines(orig, charset);
			StringBuilder str = new StringBuilder("");

			for (String line : lines) str.append(line);

			String string = str.toString();
			string = string.toLowerCase();
			string = string.replaceAll("[^a-z]", "");

			PrintWriter out = new PrintWriter(new FileOutputStream("plain.txt"), true);
			out.println(string);

		} catch (IOException e) {
      System.out.println(e);
    }

	}

	public static void encrypt() {
		BufferedReader file;
		String text, key;

		try {
			file = new BufferedReader(new FileReader("plain.txt"));
			text = file.readLine();
			file.close();

			file = new BufferedReader(new FileReader("key.txt"));
			key = file.readLine();
			file.close();

			String res = "";
			text = text.toLowerCase();
			for (int i = 0, j = 0; i < text.length(); i++) {
					char c = text.charAt(i);
					if (c < 'a' || c > 'z') continue;
					res += (char)((c + key.charAt(j) - 2 * 'a') % 26 + 'a');
					j = ++j % key.length();
			}

			PrintWriter out = new PrintWriter(new FileOutputStream("crypto.txt"), true);
			out.println(res);

		} catch (IOException e) {
      System.out.println(e);
    }

	}

	public static void decrypt() {

		BufferedReader file;
		String text, key;

		try {
			file = new BufferedReader(new FileReader("crypto.txt"));
			text = file.readLine();
			file.close();

			file = new BufferedReader(new FileReader("key.txt"));
			key = file.readLine();
			file.close();

			String res = "";
			text = text.toLowerCase();
			for (int i = 0, j = 0; i < text.length(); i++) {
					char c = text.charAt(i);
					if (c < 'a' || c > 'z') continue;
					res += (char)((c - key.charAt(j) + 26) % 26 + 'a');
					j = ++j % key.length();
			}

			PrintWriter out = new PrintWriter(new FileOutputStream("decrypt.txt"), true);
			out.println(res);

		} catch (IOException e) {
      System.out.println(e);
    }

	}

	private static String findPlaintext(int period, String ciphertext) {
		List<String> strings = new ArrayList<>(period);

		for (int i = 0; i < period; i++) {
			double minChiSquare = 99999.9;
			String bestCaesarShift = "";
			String str = "";

			for (int j = i; j < ciphertext.length(); j += period) {
				if (Character.isLetter(ciphertext.charAt(j))) {
					str += ciphertext.charAt(j);
				}
			}

			Map<Integer, String> shifts = new HashMap<>(26);
			for (int j = 0; j < 26; j++) {
				shifts.put(j, caesarShift(str, j));
			}
			for (int j = 0; j < shifts.size(); j++) {
				String shift = shifts.get(j);

				int numLetterOccurrences[] = countLetterOccurrences(shift);

				double shiftChiSquare = 0;
				for (int k = 0; k < numLetterOccurrences.length; k++) {
					double expectedFrequency = shift.length() * LETTER_FREQUENCIES[k];
					shiftChiSquare += (Math.pow((numLetterOccurrences[k] - expectedFrequency), 2) / expectedFrequency);
				}

				if (minChiSquare > shiftChiSquare) {
					minChiSquare = shiftChiSquare;
					bestCaesarShift = shift;
				}
			}
			strings.add(i, bestCaesarShift);
		}

		StringBuilder stringBuilder = new StringBuilder(ciphertext.length());
		for (int i = 0; i < strings.get(0).length(); i++) {
			for (int j = 0; j < period; j++) {
				if (strings.get(j).length() > i) {
					stringBuilder.append(strings.get(j).charAt(i));
				}
			}
		}
		return stringBuilder.toString();
	}

	private static int[] countLetterOccurrences(String str) {
		int result[] = new int[26];
		for (int k = 0; k < str.length(); k++) {
			if (Character.isLetter(str.charAt(k))) {
				result[str.charAt(k) - 'a']++;
			}
		}
		return result;
	}

	private static String caesarShift(String toShift, int numToShift) {
		String shifted = "";
		for (int i = 0; i < toShift.length(); i++) {
			char letter = (char) (toShift.charAt(i) + numToShift);
			if (letter > 'z') {
				shifted += (char) (letter - 26);
			} else {
				shifted += letter;
			}
		}
		return shifted;
	}

	private static int determineKeyPeriod(String ciphertext) {
		int period = 0;
		double indexOfCoincidence = 0.0;

		int LONGEST_KEY = 20;
		for (int i = 1; i <= LONGEST_KEY; i++) {

			String str = "";
			double stringIOC = 0.0;
			int a = 1;
			for (int j = 0; j < ciphertext.length(); j += i) {
				str += ciphertext.charAt(j);

				if (j + i >= ciphertext.length() && a != i) {
					stringIOC += determineIOC(str);
					j = a++ - i;
				} else if (j + i >= ciphertext.length()) {
					stringIOC += determineIOC(str);
				}
			}

			stringIOC /= i;

			if (stringIOC > indexOfCoincidence) {
					indexOfCoincidence = stringIOC;
					period = i;
			}
		}
		return period;
	}

	private static double determineIOC(String str) {
		int numLetters = 0, numerator = 0, denominator;

		int[] letterCounts = new int[26];
		for (int i = 0; i < letterCounts.length; i++) {
				letterCounts[i] = 0;
		}

		for (int i = 0; i < str.length(); i++) {
				if (Character.isLetter(str.charAt(i))) {
						letterCounts[str.charAt(i) - 'a']++;
						numLetters++;
				}
		}

		for (int letter : letterCounts) {
				numerator += letter * (letter - 1);
		}

		denominator = numLetters * (numLetters - 1);

		return (double) numerator / (double) denominator;
	}
//----------------------------LAST-----------------------------
	public static void cryptanalysis(){

		BufferedReader file;
		String text;

		try {
			file = new BufferedReader(new FileReader("crypto.txt"));
			text = file.readLine();
			file.close();

			int period = determineKeyPeriod(text);
			String result = findPlaintext(period, text);

			String key = "";
			for (int i = 0; i < period; i++) {
					key += (char)((text.charAt(i) - result.charAt(i) + 26) % 26 + 'a');
			}

			PrintWriter out = new PrintWriter(new FileOutputStream("key-crypto.txt"), true);
			out.println(key);

		} catch (IOException e) {
      System.out.println(e);
    }

	}


	public static void info() {
		System.out.println("\n Szyfr Vigenere'a");
		System.out.println(" autor: Wiktor Przybylowski\n");
		System.out.println("  -p = przygotowanie tekstu jawnego do szyfrowania");
		System.out.println("  -e = szyfrowanie");
		System.out.println("  -d = odszyfrowanie");
		System.out.println("  -k = kryptoanaliza wylacznie w oparciu o kryptogram\n");
	}

	public static void main(String[] args) {

		if (args.length != 1) {
			info();
		} else {
			switch (args[0]) {
				case "-p": prep();
					break;
				case "-e": encrypt();
					break;
				case "-d": decrypt();
					break;
				case "-k": cryptanalysis();
					break;
				default: info();
					break;
			}
		}
	}

}

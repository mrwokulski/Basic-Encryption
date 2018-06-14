import javax.imageio.ImageIO;
import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.File;
import java.io.FileNotFoundException;

public class block{

	//Odczytujemy obrazek i zwracamy tablice bitow obrazka
	public static int[][] imageArray(String filename){

		int[][] array = null;

		try {
			BufferedImage image = ImageIO.read(new File(filename));
			array = new int[image.getWidth()][image.getHeight()];

			for (int x=0; x<image.getWidth(); x++){
				for (int y=0; y<image.getHeight(); y++){
					int color = image.getRGB(x, y);
					if(color==Color.WHITE.getRGB())
						array[x][y] = 1;
					else
						array[x][y] = 0;
				}
			}

		} catch (IOException e) {
      System.out.println(e);
    }
		return array;
	}


	public static void ecb(int[][] array, String key) {

		int block_size = 5;

		int width = array.length;
		int height = array[0].length;
		int keycount = 0;

		for(int i=0; i<width; i+=block_size){
			for(int j=0; j<height; j+=block_size){

				for (int x=0; x<block_size; x++){
					for (int y=0; y<block_size; y++){
						array[i+x][j+y] = (array[i+x][j+y]^(int)key.charAt(keycount))%2;
						keycount++;
						if( keycount == key.length() )
							keycount = 0;
					}
				}

			}
		}
		saveImage(array, "ecb_crypto.bmp");
	}


	public static void cbc(int[][] array, String key) {

		int block_x = 5;
		int block_y = 10;

		int width = array.length;
		int height = array[0].length;
		int keycount = 0;
		int[] vector = { 120, 49, 13, 7, 41, 576, 217, 93 };

		for(int i=0; i<width; i+=block_x){
			for(int j=0; j<height; j+=block_y){

				for (int x=0; x<block_x; x++){
					for (int y=0; y<block_y; y++){
						array[i+x][j+y] += vector[x];
						array[i+x][j+y] = (array[i+x][j+y]^(int)key.charAt(keycount)) % 2;
						vector[x] = array[i+x][j+y];
						keycount++;
						if(keycount == key.length())
							keycount = 0;
					}
				}
			}
		}
		saveImage(array, "cbc_crypto.bmp");
	}

	//Zapisujemy obraz na podstawie otrzymanej tablicy bitow
	public static void saveImage(int[][] array, String filename){

		int width = array.length;
		int height = array[0].length;

		BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_BYTE_BINARY);
		for (int x=0; x<image.getWidth(); x++){
			for (int y=0; y<image.getHeight(); y++){
				if(array[x][y]==1)
					image.setRGB(x, y, Color.WHITE.getRGB());
				else
					image.setRGB(x, y, Color.BLACK.getRGB());
			}
		}
		try {
			ImageIO.write(image, "bmp", new File(filename));
		} catch (IOException e) {
      System.out.println(e);
    }

	}

	public static void main(String[] args) {

		String filename = "plain.bmp";
		String key = "FL8By>Tb#X_Z@=L0t=Johk?e30GK<Q";

		int[][] array = imageArray(filename);
		ecb(array, key);
		cbc(array, key);
	}

}

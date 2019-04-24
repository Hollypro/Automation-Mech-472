
// maximum image size (in bytes)
#define MAX_IMAGE 927000

const int IMAGE_WIDTH=640;
const int IMAGE_HEIGHT=480;

// image types
#define RGB_IMAGE 1
#define GREY_IMAGE 2
#define LABEL_IMAGE 3

// define a variable type that holds a single byte of data
typedef unsigned char ibyte;
typedef unsigned short int i2byte;
typedef unsigned long int i4byte;

// define a structure that stores an image
typedef struct {
	int type;   // image type
	i2byte height; // image height
	i2byte width;  // image width
	ibyte *pdata; // pointer to image data
	i2byte nlabels;  // number of labels (used for LABEL_IMAGE type)
} image;


// allocate image memory
int allocate_image(image &a);

// free image memory
int free_image(image &a);

// acquire image from a video source
int acquire_image(image &a);
int acquire_image(image &a, double &t); // returns sample time

// initialize Vision library
int activate_vision(int camera);

int deactivate_vision();

// view the image in the window created by view_image.exe
int view_image(image &a);

// save the image into a bitmap (*.bmp) file
int save_image(char *file_name, image &a);

// load the image from a bitmap (*.bmp) file
int load_image(char *file_name, image &a);



//	***Our functions***

int setup(); //setup camera, images, can add more

int activate_process(); // process image initialization
		// executed at program startup before image acquisition takes place

int process_image(); // process each incoming image

int deactivate_process(); // process image deactivation
		// executed at program shutdown after image surrender takes place

int rgb_detection(image rgb_in, image &rgb_out); // basic script to detect RGB and grey everything out

int shutdown(); //deallocate cameras, images, --opposite of setup()--
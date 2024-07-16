#include <GL/gl.h>
#include <cglm/types-struct.h>

#include <GLFW/glfw3.h>
#include <leif/leif.h>

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/config.h"
#include "../include/read.h"
#include "../include/change.h"

/* TODO: if(pathInput is an image then display else display NO IMAGE) */



int main() {
  glfwInit();
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  GLFWwindow* window = glfwCreateWindow(WIN_X, WIN_Y, WIN_TITLE, NULL, NULL);

  glfwMakeContextCurrent(window);

  lf_init_glfw(1280, 720, window);

  // Title Font 
  LfFont titleFont = lf_load_font(FONT_BOLD, TITLE_FONT_SIZE);

  // Subtitle Font 
  LfFont subTitleFont = lf_load_font(FONT, TITLE_FONT_SIZE / 2);
  
  // Button Font
  LfFont buttonFont = lf_load_font(FONT,  BUTTON_FONT_SIZE);

  // Normal Font 
  LfFont normalFont = lf_load_font(FONT, NORMAL_FONT_SIZE);

  vec4s bgColor = lf_color_to_zto(BG_COLOR);
  
  // Input 
  LfInputField pathInput;
  char pathInputBuf[PATH_INPUT_BUFF_SIZE];

  memset(pathInputBuf, 0, PATH_INPUT_BUFF_SIZE);
  pathInput = (LfInputField){
    .width = 450,
    .buf = pathInputBuf,
    .buf_size = PATH_INPUT_BUFF_SIZE,
    .placeholder = (char*)"/C-rypto/..."
  };

  LfInputField textToCrypt;
  char textToCryptInputBuf[PATH_INPUT_BUFF_SIZE];

  memset(textToCryptInputBuf, 0, PATH_INPUT_BUFF_SIZE);
  textToCrypt = (LfInputField){
    .width = 700,
    .start_height = WIN_Y - 164,
    .buf = textToCryptInputBuf,
    .buf_size = PATH_INPUT_BUFF_SIZE,
    .placeholder = (char*)"Who lives in a pineapple under the sea? ..."
  };

  int isImage = 0;
  LfTexture selectedImage = lf_load_texture(NO_PNG, true, LF_TEX_FILTER_LINEAR);

  char* oldBuf = malloc(sizeof(pathInputBuf));
  strcpy(oldBuf, pathInputBuf);

  int porcoddio_count = 0;

  // Main loop
  while(!glfwWindowShouldClose(window)) {
    if((strcmp(oldBuf, pathInputBuf)) != 0) {
      porcoddio_count++;
      printf("==%d==\npathInputBuf: %s\noldBuf:%s\n", porcoddio_count, pathInputBuf, (char*)oldBuf);
      if(ImageExist((char*)pathInputBuf)) {
        selectedImage = lf_load_texture(pathInputBuf, true, LF_TEX_FILTER_LINEAR);
        isImage = 1;
      } else {
        isImage = 0;
        selectedImage = lf_load_texture(NO_PNG, true, LF_TEX_FILTER_LINEAR);
      }
      strcpy(oldBuf, pathInputBuf);
    }

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    
    lf_begin();
    
    // Print Title
    {
      LfUIElementProps titleprops = lf_get_theme().text_props;
      titleprops.text_color = LF_WHITE;
      
      lf_push_style_props(titleprops);

      lf_push_font(&titleFont);
  
      lf_set_ptr_x_absolute(10);
      lf_set_ptr_y_absolute(10);
      
      lf_text(APP_TITLE);

  
      lf_pop_style_props(&titleprops);
      lf_pop_font(&titleFont);
    }

    

    // Path input


    { 
      lf_push_font(&subTitleFont);
      lf_set_ptr_x_absolute(WIN_X - 500);
      lf_set_ptr_y_absolute(70);
      lf_text("Image Path");
      lf_pop_font(subTitleFont);
      lf_set_ptr_x_absolute(WIN_X - 500);
      lf_set_ptr_y_absolute(WIDGET_START_Y + lf_text_dimension("Input / Output").y * 2);
      LfUIElementProps props = lf_get_theme().inputfield_props;
      props.padding = 15;
      props.border_width = 0;
      props.color = BG_COLOR;
      props.corner_radius = 11;
      props.text_color = LF_WHITE;
      props.border_width = 1.0f;
      props.border_color = pathInput.selected ? LF_WHITE : (LfColor){170, 170, 170, 255};
      props.corner_radius = 2.5f;
      props.margin_bottom = 10.0f;
      lf_push_style_props(props);
      lf_input_text(&pathInput);
      lf_pop_style_props();
      
      lf_input_field_unselect_all(&pathInput);
      lf_pop_font(&normalFont);
    }
  
    // Title of Crypt/Decrypt  box

    lf_push_font(&subTitleFont);
    lf_set_ptr_x_absolute(10);
    lf_set_ptr_y_absolute(70);
    lf_text("Input / Output");
    lf_pop_font(&subTitleFont);

    // To crypt Decrypted text box
    { 
      lf_push_font(&normalFont);
      lf_set_ptr_x_absolute(10);
      lf_set_ptr_y_absolute(WIDGET_START_Y + lf_text_dimension("Input / Output").y * 2 + 4);
      LfUIElementProps props = lf_get_theme().inputfield_props;
      props.padding = 15;
      props.border_width = 0;
      props.color = BG_COLOR;
      props.corner_radius = 11;
      props.text_color = LF_WHITE;
      props.border_width = 1.0f;
      props.border_color = textToCrypt.selected ? LF_WHITE : (LfColor){170, 170, 170, 255};
      props.corner_radius = 2.5f;
      props.margin_bottom = 10.0f;
      lf_push_style_props(props);
      lf_input_text(&textToCrypt);
      lf_pop_style_props();
      
      lf_input_field_unselect_all(&textToCrypt);
      lf_pop_font(&normalFont);
    }
  


    
    // Print Button
    {
      // Define Button - crypt & decrypt 
      LfUIElementProps Cbtnprops = lf_get_theme().button_props;
      Cbtnprops.margin_left = 10.0f; Cbtnprops.margin_top = 15.0f; Cbtnprops.border_width = 0.0f; Cbtnprops.corner_radius = 2.0f;
      Cbtnprops.text_color = LF_BLACK;
      Cbtnprops.color = strlen(textToCryptInputBuf) && strlen(pathInputBuf) && isImage ? LF_WHITE : (LfColor){151, 153, 154, 255};    
      lf_set_ptr_x_absolute(200);
      lf_set_ptr_y_absolute(700);

      LfUIElementProps Dbtnprops = lf_get_theme().button_props;
      Dbtnprops.margin_left = 10.0f; Dbtnprops.margin_top = 15.0f; Dbtnprops.border_width = 0.0f; Dbtnprops.corner_radius = 2.0f;
      Dbtnprops.text_color = LF_BLACK;
      Dbtnprops.color = strlen(pathInputBuf) &&  isImage ? LF_WHITE : (LfColor){151, 153, 154, 255};    
      lf_set_ptr_x_absolute(200);
      lf_set_ptr_y_absolute(700);


    
      lf_push_font(&buttonFont);
      lf_push_style_props(Cbtnprops);
    
      lf_set_ptr_x_absolute(WIN_X - 230);
      lf_set_ptr_y_absolute(5);

if (lf_button_fixed(OPTION_1, 60, -1) == LF_CLICKED && isImage && (strlen(textToCryptInputBuf))) {
    PixelRGBA wordToPixel[40];

    for (int x = 0; x < 40; x++) {
        if (textToCryptInputBuf[x] == '\0') { 
            break;
        }

        switch (textToCryptInputBuf[x]) {
            case 'a':
            case 'A':
                wordToPixel[x] = (PixelRGBA)RGBA_A;
                break;
            case 'b':
            case 'B':
                wordToPixel[x] = (PixelRGBA)RGBA_B;
                break;
            case 'c':
            case 'C':
                wordToPixel[x] = (PixelRGBA)RGBA_C;
                break;
            case 'd':
            case 'D':
                wordToPixel[x] = (PixelRGBA)RGBA_D;
                break;
            case 'e':
            case 'E':
                wordToPixel[x] = (PixelRGBA)RGBA_E;
                break;
            case 'f':
            case 'F':
                wordToPixel[x] = (PixelRGBA)RGBA_F;
                break;
            case 'g':
            case 'G':
                wordToPixel[x] = (PixelRGBA)RGBA_G;
                break;
            case 'h':
            case 'H':
                wordToPixel[x] = (PixelRGBA)RGBA_H;
                break;
            case 'i':
            case 'I':
                wordToPixel[x] = (PixelRGBA)RGBA_I;
                break;
            case 'j':
            case 'J':
                wordToPixel[x] = (PixelRGBA)RGBA_J;
                break;
            case 'k':
            case 'K':
                wordToPixel[x] = (PixelRGBA)RGBA_K;
                break;
            case 'l':
            case 'L':
                wordToPixel[x] = (PixelRGBA)RGBA_L;
                break;
            case 'm':
            case 'M':
                wordToPixel[x] = (PixelRGBA)RGBA_M;
                break;
            case 'n':
            case 'N':
                wordToPixel[x] = (PixelRGBA)RGBA_N;
                break;
            case 'o':
            case 'O':
                wordToPixel[x] = (PixelRGBA)RGBA_O;
                break;
            case 'p':
            case 'P':
                wordToPixel[x] = (PixelRGBA)RGBA_P;
                break;
            case 'q':
            case 'Q':
                wordToPixel[x] = (PixelRGBA)RGBA_Q;
                break;
            case 'r':
            case 'R':
                wordToPixel[x] = (PixelRGBA)RGBA_R;
                break;
            case 's':
            case 'S':
                wordToPixel[x] = (PixelRGBA)RGBA_S;
                break;
            case 't':
            case 'T':
                wordToPixel[x] = (PixelRGBA)RGBA_T;
                break;
            case 'u':
            case 'U':
                wordToPixel[x] = (PixelRGBA)RGBA_U;
                break;
            case 'v':
            case 'V':
                wordToPixel[x] = (PixelRGBA)RGBA_V;
                break;
            case 'w':
            case 'W':
                wordToPixel[x] = (PixelRGBA)RGBA_W;
                break;
            case 'x':
            case 'X':
                wordToPixel[x] = (PixelRGBA)RGBA_X;
                break;
            case 'y':
            case 'Y':
                wordToPixel[x] = (PixelRGBA)RGBA_Y;
                break;
            case 'z':
            case 'Z':
                wordToPixel[x] = (PixelRGBA)RGBA_Z;
                break;
            default:
                // Carattere non gestito, puoi inserire un valore di default o gestire l'errore
                fprintf(stderr, "Carattere non supportato: %c\n", textToCryptInputBuf[x]);
                break;
        }
    }
        char* outFileName = malloc(120);
        char* suxcessMessage = malloc(132);
        sprintf(outFileName, "%s%s", pathInputBuf, "-secret.png");
        change_pixels(pathInputBuf, outFileName, wordToPixel, 20); // TODO change 20
        memset(textToCryptInputBuf, '\0', sizeof(textToCryptInputBuf));
        sprintf(suxcessMessage, "Crypted image: %s", outFileName);
        textToCrypt.placeholder = suxcessMessage;
      }
    
      lf_push_style_props(Dbtnprops);

      if(lf_button_fixed(OPTION_2, 89, -1) == LF_CLICKED && isImage) {
        char* decodedString = malloc(40 /* MAX WORD SIZE*/);
        memset(textToCryptInputBuf, '\0', sizeof(textToCryptInputBuf));
        if(IsValid(pathInputBuf)) {
          decodedString = generateEncodedAlphabet(pathInputBuf);
        } 
        textToCrypt.placeholder = decodedString;
      }
      lf_pop_style_props(&Cbtnprops);
      lf_pop_style_props(&Dbtnprops);
      lf_pop_font(&buttonFont);
    }


    // Display image

    lf_set_ptr_x_absolute(100);
    lf_set_ptr_y_absolute(100);

    selectedImage.width = 500;
    selectedImage.height = 500;

    if(lf_image_button(((LfTexture){.id = selectedImage.id}))) {
      ;
    }


    lf_end();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  lf_terminate();
  glfwDestroyWindow(window);
  glfwTerminate();
}

/*
int main() {
  int MAX_BUFF = 400;
  PixelRGBA wordToPixel[MAX_BUFF];
  for(int x = 0; x < 10; x++) {
    wordToPixel[x] = (PixelRGBA)RGBA_C;
  }
  wordToPixel[9] = (PixelRGBA)RGBA_F;
  wordToPixel[10] = (PixelRGBA)RGBA_END;

  change_pixels("lain1.png", "lain.png", wordToPixel, 11);  

}*/

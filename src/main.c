#include <GL/gl.h>
#include <cglm/types-struct.h>

#include <GLFW/glfw3.h>
#include <leif/leif.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/config.h"
#include "../include/read.h"

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
  char pathInputBuf[PATH_INPUT_BUF_SIZE];

  memset(pathInputBuf, 0, PATH_INPUT_BUF_SIZE);
  pathInput = (LfInputField){
    .width = 450,
    .buf = pathInputBuf,
    .buf_size = PATH_INPUT_BUF_SIZE,
    .placeholder = (char*)"/C-rypto/..."
  };

  LfInputField textToCrypt;
  char textToCryptInputBuf[PATH_INPUT_BUF_SIZE];

  memset(textToCryptInputBuf, 0, PATH_INPUT_BUF_SIZE);
  textToCrypt = (LfInputField){
    .width = 700,
    .start_height = WIN_Y - 164,
    .buf = textToCryptInputBuf,
    .buf_size = PATH_INPUT_BUF_SIZE,
    .placeholder = (char*)"Who lives in a pineapple under the sea? ..."
  };

  int isImage = 0;

  while(!glfwWindowShouldClose(window)) {
    if(ImageExsist((char*)pathInputBuf)) {
      isImage = 1;
    } else {
      isImage = 0;
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

      if(lf_button_fixed(OPTION_1, 60, -1) == LF_CLICKED) {
        glfwSetWindowShouldClose(window, 1);
      }
    
      lf_push_style_props(Dbtnprops);

      if(lf_button_fixed(OPTION_2, 89, -1) == LF_CLICKED) {
        glfwSetWindowShouldClose(window, 1);
      }
      lf_pop_style_props(&Cbtnprops);
      lf_pop_style_props(&Dbtnprops);
      lf_pop_font(&buttonFont);
    }


    lf_end();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  lf_terminate();
  glfwDestroyWindow(window);
  glfwTerminate();
}


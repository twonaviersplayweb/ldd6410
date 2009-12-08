// generated by Fast Light User Interface Designer (fluid) version 1.00

#ifndef function_panel_h
#define function_panel_h
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
extern Fl_Window *function_panel;
#include <FL/Fl_Light_Button.H>
extern Fl_Light_Button *f_public_button;
extern Fl_Light_Button *f_c_button;
#include <FL/Fl_Input.H>
extern Fl_Input *f_name_input;
extern Fl_Input *f_return_type_input;
#include <FL/Fl_Return_Button.H>
extern Fl_Return_Button *f_panel_ok;
#include <FL/Fl_Button.H>
extern Fl_Button *f_panel_cancel;
Fl_Window* make_function_panel();
extern Fl_Window *code_panel;
extern Fl_Input *code_input;
extern Fl_Return_Button *code_panel_ok;
extern Fl_Button *code_panel_cancel;
Fl_Window* make_code_panel();
extern Fl_Window *codeblock_panel;
extern Fl_Input *code_before_input;
#include <FL/Fl_Box.H>
extern Fl_Input *code_after_input;
extern Fl_Return_Button *codeblock_panel_ok;
extern Fl_Button *codeblock_panel_cancel;
Fl_Window* make_codeblock_panel();
extern Fl_Window *declblock_panel;
extern Fl_Input *decl_before_input;
extern Fl_Input *decl_after_input;
extern Fl_Return_Button *declblock_panel_ok;
extern Fl_Button *declblock_panel_cancel;
Fl_Window* make_declblock_panel();
extern Fl_Window *decl_panel;
extern Fl_Light_Button *decl_public_button;
extern Fl_Input *decl_input;
extern Fl_Return_Button *decl_panel_ok;
extern Fl_Button *decl_panel_cancel;
Fl_Window* make_decl_panel();
extern Fl_Window *class_panel;
extern Fl_Light_Button *c_public_button;
extern Fl_Input *c_name_input;
extern Fl_Input *c_subclass_input;
extern Fl_Return_Button *c_panel_ok;
extern Fl_Button *c_panel_cancel;
Fl_Window* make_class_panel();
#endif

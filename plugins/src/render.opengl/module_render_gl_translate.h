class module_render_gl_translate : public vsx_module
{
  // in
  GLfloat tmpMat[16];

  vsx_module_param_float3* translation;

  vsx_module_param_render* render_in;

  // out
  vsx_module_param_render* render_result;

  // internal
  vsx_gl_state* gl_state;

public:

  void module_info(vsx_module_specification* info)
  {
    info->identifier =
      "renderers;opengl_modifiers;gl_translate";

    info->description =
      "Moves or \"translates\"\n"
      "an object in x,y,z directions.\n"
      "Use this if you want to move\n"
      "an object around or if the \n"
      "renderer doesn't have a position\n"
      "parameter. (Some don't as \n"
      "this module exists, it's more\n"
      "efficcient to use matrix transforms."
    ;

    info->in_param_spec =
      "render_in:render,"
      "translation:float3"
    ;

    info->out_param_spec =
      "render_out:render";

    info->component_class =
      "render";

    info->tunnel = true; // always run this
  }


  void declare_params(vsx_module_param_list& in_parameters, vsx_module_param_list& out_parameters)
  {
    loading_done = true;
    translation = (vsx_module_param_float3*)in_parameters.create(VSX_MODULE_PARAM_ID_FLOAT3,"translation");

    render_in = (vsx_module_param_render*)in_parameters.create(VSX_MODULE_PARAM_ID_RENDER,"render_in");
    render_in->run_activate_offscreen = true;

    render_result = (vsx_module_param_render*)out_parameters.create(VSX_MODULE_PARAM_ID_RENDER,"render_out");

    gl_state = vsx_gl_state::get();
  }

  bool activate_offscreen()
  {
    // save current matrix
    gl_state->matrix_get_v(VSX_GL_MODELVIEW_MATRIX, tmpMat);
    gl_state->matrix_mode(VSX_GL_MODELVIEW_MATRIX);
    gl_state->matrix_translate_f(translation->get(0),translation->get(1),translation->get(2));
    return true;
  }

  void deactivate_offscreen()
  {
    // reset the matrix to previous value
    gl_state->matrix_mode(VSX_GL_MODELVIEW_MATRIX);
    gl_state->matrix_load_identity();
    gl_state->matrix_mult_f(tmpMat);
  }

  void output(vsx_module_param_abs* param)
  {
    VSX_UNUSED(param);
    render_result->set(render_in->get());
  }

};



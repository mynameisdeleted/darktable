/*
   This file is part of darktable,
   copyright (c) 2015 Jeremy Rosen

   darktable is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   darktable is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with darktable.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "lua/widget.h"
#include "lua/types.h"

typedef struct {
  dt_lua_widget_t parent;
  char * default_label;
  gboolean default_value;
} dt_lua_check_button_t;

typedef dt_lua_check_button_t* lua_check_button;

lua_widget check_button_init(lua_State* L);
void check_button_reset(lua_widget widget);
void check_button_cleanup(lua_State* L,lua_widget widget);
static dt_lua_widget_type_t check_button_type = {
  .name = "check_button",
  .gui_init = check_button_init,
  .gui_reset = check_button_reset,
  .gui_cleanup = check_button_cleanup,
};

lua_widget check_button_init(lua_State* L)
{
  const char * new_value = NULL;
  if(!lua_isnoneornil(L,1)){
    new_value = luaL_checkstring(L,1);
  }
  lua_check_button check_button = malloc(sizeof(dt_lua_check_button_t));
  if(new_value) {
    check_button->default_label = strdup(new_value);
    check_button->parent.widget = gtk_check_button_new_with_label(new_value);
  } else {
    check_button->default_label = NULL;
    check_button->parent.widget = gtk_check_button_new();
  }
  check_button->default_value = lua_toboolean(L,2);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_button->parent.widget),lua_toboolean(L,2));
  return (lua_widget) check_button;
}

void check_button_reset(lua_widget widget)
{
  lua_check_button check_button = (lua_check_button)widget;
  gtk_button_set_label(GTK_BUTTON(check_button->parent.widget),check_button->default_label);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_button->parent.widget),check_button->default_value);
}

void check_button_cleanup(lua_State* L,lua_widget widget)
{
  lua_check_button check_button = (lua_check_button)widget;
  if(check_button->default_label) free(check_button->default_label);
}

static int default_label_member(lua_State *L)
{
  lua_check_button check_button;
  luaA_to(L,lua_check_button,&check_button,1);
  if(lua_gettop(L) > 2) {
    const char * new_val =luaL_checkstring(L,3);
    free(check_button->default_label);
    check_button->default_label = strdup(new_val);
    return 0;
  }
  lua_pushstring(L,check_button->default_label);
  return 1;
}

static int label_member(lua_State *L)
{
  lua_check_button check_button;
  luaA_to(L,lua_check_button,&check_button,1);
  if(lua_gettop(L) > 2) {
    const char * label = luaL_checkstring(L,3);
    gtk_button_set_label(GTK_BUTTON(check_button->parent.widget),label);
    return 0;
  }
  lua_pushstring(L,gtk_button_get_label(GTK_BUTTON(check_button->parent.widget)));
  return 1;
}

static int value_member(lua_State *L)
{
  lua_check_button check_button;
  luaA_to(L,lua_check_button,&check_button,1);
  if(lua_gettop(L) > 2) {
    luaL_checktype(L,3,LUA_TBOOLEAN);
    gboolean value = lua_toboolean(L,3);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_button->parent.widget),value);
    return 0;
  }
  lua_pushboolean(L,gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button->parent.widget)));
  return 1;
}

static int default_value_member(lua_State *L)
{
  lua_check_button check_button;
  luaA_to(L,lua_check_button,&check_button,1);
  if(lua_gettop(L) > 2) {
    gboolean new_val =lua_toboolean(L,3);
    check_button->default_value = new_val;
    return 0;
  }
  lua_pushboolean(L,check_button->default_value);
  return 1;
}

int dt_lua_init_widget_check_button(lua_State* L)
{
  dt_lua_init_gpointer_type(L,lua_check_button);
  dt_lua_register_widget_type(L,&check_button_type,lua_check_button);

  lua_pushcfunction(L,value_member);
  lua_pushcclosure(L,dt_lua_gtk_wrap,1);
  dt_lua_type_register(L, lua_check_button, "value");
  lua_pushcfunction(L,default_value_member);
  lua_pushcclosure(L,dt_lua_gtk_wrap,1);
  dt_lua_type_register(L, lua_check_button, "default_value");
  lua_pushcfunction(L,label_member);
  lua_pushcclosure(L,dt_lua_gtk_wrap,1);
  dt_lua_type_register(L, lua_check_button, "label");
  lua_pushcfunction(L,default_label_member);
  lua_pushcclosure(L,dt_lua_gtk_wrap,1);
  dt_lua_type_register(L, lua_check_button, "default_label");

  return 0;
}
// modelines: These editor modelines have been set for all relevant files by tools/update_modelines.sh
// vim: shiftwidth=2 expandtab tabstop=2 cindent
// kate: tab-indents: off; indent-width 2; replace-tabs on; indent-mode cstyle; remove-trailing-space on;

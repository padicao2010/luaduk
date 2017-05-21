#include "lua.h"
#include "lauxlib.h"
#include "duktape.h"

static duk_ret_t native_print(duk_context *ctx) {                               
    duk_push_string(ctx, " ");                                                  
    duk_insert(ctx, 0);                                                         
    duk_join(ctx, duk_get_top(ctx) - 1);                                        
    printf("%s\n", duk_safe_to_string(ctx, -1));                                
    return 0;                                                                   
}

static int return_error(lua_State *L, const char *s) {
    lua_pushnil(L);
    lua_pushstring(L, s);
    return 2;
}

static int return_success(lua_State *L) {
    lua_pushboolean(L, 1);
    return 1;
}

/* ctx, err = lduk.new() */
static int lduk_new(lua_State *L) {
    duk_context *ctx = duk_create_heap_default();
    if(!ctx)
        return return_error(L, "Can't create heap!");
    
    duk_push_c_function(ctx, native_print, DUK_VARARGS);                        
    duk_put_global_string(ctx, "print");
    
    lua_pushlightuserdata(L, ctx);
    return 1;
}

/* lduk.free(ctx) */
static int lduk_free(lua_State *L) {
    duk_context *ctx = (duk_context *)lua_topointer(L, -1);

    if(!ctx)
        duk_destroy_heap(ctx);

    return 0;
}

/* status, err = lduk.eval(ctx, codes) */
static int lduk_eval(lua_State *L) {
    const char *code = luaL_checkstring(L, -1);
    duk_context *ctx = (duk_context *)lua_topointer(L, -2);
    
    if(!ctx)
        return return_error(L, "CTX is null!");

    if(0 != duk_peval_string_noresult(ctx, code)) {
        const char *errmsg = duk_safe_to_string(ctx, -1);
        return return_error(L, errmsg);
    }
    
    return return_success(L);
}

/* lduk.pushfunc(ctx, funcname) */
static int lduk_pushfunc(lua_State *L) {
    duk_context *ctx = (duk_context *)lua_topointer(L, -2);
    const char *fname = luaL_checkstring(L, -1);
    
    if(!ctx)
        return return_error(L, "CTX is null!");
        
    duk_get_global_string(ctx, fname);
    
    return return_success(L);
}

/* lduk.pushinteger(ctx, 10) */
static int lduk_pushinteger(lua_State *L) {
    duk_context *ctx = (duk_context *)lua_topointer(L, -2);
    int x = luaL_checkinteger(L, -1);
    
    if(!ctx)
        return return_error(L, "CTX is null!");
        
    duk_push_int(ctx, x);
    
    return return_success(L);
}

/* lduk.pushnumber(ctx, 20.0) */
static int lduk_pushnumber(lua_State *L) {
    duk_context *ctx = (duk_context *)lua_topointer(L, -2);
    double x = luaL_checknumber(L, -1);
    
    if(!ctx)
        return return_error(L, "CTX is null!");
        
    duk_push_number(ctx, x);
    
    return return_success(L);
}

/* lduk.pushstring(ctx, "Hello") */
static int lduk_pushstring(lua_State *L) {
    duk_context *ctx = (duk_context *)lua_topointer(L, -2);
    const char *s = luaL_checkstring(L, -1);
    
    if(!ctx)
        return return_error(L, "CTX is null!");
        
    duk_push_string(ctx, s);
    
    return return_success(L);
}

/* i = lduk.popinteger(ctx) */
static int lduk_popinteger(lua_State *L) {
    duk_context *ctx = (duk_context *)lua_topointer(L, -1);
    
    if(!ctx)
        return return_error(L, "CTX is null!");
        
    int x = duk_get_int(ctx, -1);
    duk_pop(ctx);
    
    lua_pushinteger(L, x);
    return 1;
}

/* f = lduk.popnumber(ctx) */
static int lduk_popnumber(lua_State *L) {
    duk_context *ctx = (duk_context *)lua_topointer(L, -1);
    
    if(!ctx)
        return return_error(L, "CTX is null!");
        
    double x = duk_get_number(ctx, -1);
    duk_pop(ctx);
    
    lua_pushnumber(L, x);
    return 1;
}

/* s = lduk.popstring(ctx) */
static int lduk_popstring(lua_State *L) {
    duk_context *ctx = (duk_context *)lua_topointer(L, -1);
    
    if(!ctx)
        return return_error(L, "CTX is null!");
        
    const char* x = duk_get_string(ctx, -1);
    duk_pop(ctx);
    
    lua_pushstring(L, x);
    return 1;
}

/* lduk.call(ctx, nargs) */
static int lduk_call(lua_State *L) {
    duk_context *ctx = (duk_context *)lua_topointer(L, -2);
    int nargs = luaL_checknumber(L, -1);
    
    if(0 != duk_pcall(ctx, nargs)) {
        const char *errmsg = duk_safe_to_string(ctx, -1);
        return return_error(L, errmsg);
    }
    
    return return_success(L);
}

static const struct luaL_Reg cfunctions [] = {
    {"new", lduk_new},
    {"free", lduk_free},
    {"eval", lduk_eval},
    {"pushfunc", lduk_pushfunc},
    {"pushinteger", lduk_pushinteger},
    {"pushnumber", lduk_pushnumber},
    {"pushstring", lduk_pushstring},
    {"popinteger", lduk_popinteger},
    {"popnumber", lduk_popnumber},
    {"popstring", lduk_popstring},
    {"call", lduk_call},
    
    {NULL, NULL}
};

int luaopen_lduk(lua_State *L) {
    luaL_newlibtable(L, cfunctions);
    luaL_setfuncs(L, cfunctions, 0);
    return 1;
}

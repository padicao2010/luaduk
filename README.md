# Lua binding for Duktape

*lduk* is a simple lua-binding for Duktape.

* Duktape v2.0.3
* lua v5.3.4

# lduk API

Start with:

```
local lduk = require "lduk"
```

* ctx = lduk.new()
* lduk.free(ctx)
* lduk.eval(ctx, jsstring): WARNING: NO RETVALS.
* lduk.pushinteger(ctx, 1)
* lduk.pushnumber(ctx, 1.0)
* lduk.pushstring(ctx, "1")
* lduk.call(ctx, nargs)
* i = lduk.popinteger(ctx)
* f = lduk.popnumber(ctx)
* s = lduk.popstring(ctx)

# An example

```
local lduk = require("lduk")

local ctx = assert(lduk.new())
local s = [[
function a(x,y) {
        return x+y;
}
]]
lduk.eval(ctx, s)

-- call a(10, 20) and get retval.
lduk.pushfunc(ctx, "a")
lduk.pushinteger(ctx, 10)
lduk.pushinteger(ctx, 20)
lduk.call(ctx, 2)
print(lduk.popinteger(ctx))

lduk.free(ctx)
```

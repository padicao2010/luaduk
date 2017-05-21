local lduk = require("lduk")

ctx = assert(lduk.new())
s = [[
function a(x,y) {
        return x+y;
}
]]
lduk.eval(ctx, s)

lduk.pushfunc(ctx, "a")
lduk.pushinteger(ctx, 10)
lduk.pushinteger(ctx, 20)
lduk.call(ctx, 2)
print(lduk.popinteger(ctx))

lduk.pushfunc(ctx, "a")
lduk.pushnumber(ctx, 10)
lduk.pushnumber(ctx, 20)
lduk.call(ctx, 2)
print(lduk.popnumber(ctx))

lduk.pushfunc(ctx, "a")
lduk.pushstring(ctx, 10)
lduk.pushstring(ctx, 20)
lduk.call(ctx, 2)
print(lduk.popstring(ctx))

lduk.free(ctx)

#ifndef RAT_LUA_H
#define RAT_LUA_H

#define LUA_ARG_COUNT(NUM, NAME) {                                             \
	if (lua_gettop(L) != NUM) {                                                \
		printf("lua error: function '%s' passed %d arguments, but takes %d\n", \
		       NAME, lua_gettop(L), NUM);                                      \
		exit(EXIT_FAILURE);                                                    \
	}                                                                          \
}

#define LUA_SET_VAR(VAR) {  \
	lua_pushnumber(L, VAR); \
	lua_setglobal(L, #VAR); \
}

#define LUA_LOAD_FILE(F) {                            \
	if (luaL_dofile(L, F)) {                          \
		fprintf(stderr, "%s\n", lua_tostring(L, -1)); \
		lua_pop(L, 1);                                \
		exit(EXIT_FAILURE);                           \
	}                                                 \
}

#define LUA_LOAD_HEADER(H, F, FLEN) {                   \
	if (luaL_loadbuffer(L, (const char *)F, FLEN, H) || \
	    lua_pcall(L, 0, LUA_MULTRET, 0)) {              \
		fprintf(stderr, "%s\n", lua_tostring(L, -1));   \
		lua_pop(L, 1);                                  \
		exit(EXIT_FAILURE);                             \
	}                                                   \
}

#endif

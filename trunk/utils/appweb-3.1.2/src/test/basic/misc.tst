/*
 *  misc.tst - Misc. Http tests
 */

require ejs.test

assert(Http.mimeType("a.txt") == "text/plain")
assert(Http.mimeType("a.html") == "text/html")

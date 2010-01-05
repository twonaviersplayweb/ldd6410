/*
 *  badUrl.tst - Stress test malformed URLs 
 */

require ejs.test

const HTTP = session["main"]
let http: Http = new Http

//  TODO - this will be more meaningful when ejs supports unicode
http.get(HTTP + "/index\x00.html")
assert(http.code == 404)
assert(http.response.contains("Not Found"))

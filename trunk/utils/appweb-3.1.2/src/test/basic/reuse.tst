/*
 *  reuse.tst - Test Http reuse for multiple requests
 */

require ejs.test

const HTTP = session["main"]
const URL = HTTP + "/index.html"

let http: Http = new Http

http.get(URL)
assert(http.code == 200)
http.get(URL)
assert(http.code == 200)
http.get(URL)
assert(http.code == 200)
http.get(URL)
assert(http.code == 200)

/*
 *  chunk.tst - Test chunked transfer encoding for response data
 */

require ejs.test

let HTTP = session["main"]
let http: Http = new Http

/*
http.chunked = true
http.post(HTTP + "/index.html")
http.wait()
// print("CODE " + http.code)
assert(http.code == 200)
*/

http.chunked = false
http.post(HTTP + "/index.html")
http.wait()
// print("CODE " + http.code)
assert(http.code == 200)

//  TODO - more here. Test various chunk sizes.
//  TODO - want to be able to set the chunk size?

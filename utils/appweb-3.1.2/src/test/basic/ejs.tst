/*
 *  ejs.tst - EJS basic tests
 */

require ejs.test

const HTTP = session["main"]
let http: Http = new Http

/* Suport routines */

function contains(pat): Void {
    assert(http.response.contains(pat))
}

function keyword(pat: String): String {
    pat.replace(/\//, "\\/").replace(/\[/, "\\[")
    let reg = RegExp('/.*"' + pat + '": "*([^",]*).*/s')
    return http.response.replace(reg, "$1")
}

function match(key: String, value: String): Void {
    assert(keyword(key) == value)
}


/* Tests */

function basic() {
    http.get(HTTP + "/ejsProgram.ejs")
    assert(http.code == 200)
    contains("ejsProgram: EJS Output")
}

function forms() {
    http.get(HTTP + "/ejsProgram.ejs")
    assert(http.code == 200)
    contains("ejsProgram: EJS Output")

    //  TODO - URLs are case sensitive inside the MVC
    if (false && test.os == "WIN") {
        http.get(HTTP + "/ejsProgRAM.eJS")
        assert(http.code == 200)
    }
}

function alias() {
    http.get(HTTP + "/SimpleAlias/ejsProgram.ejs")
    assert(http.code == 200)
    contains("ejsProgram: EJS Output")
    match("url", "/SimpleAlias/ejsProgram.ejs")
    match("query", "null")
}

function query() {
    http.get(HTTP + "/ejsProgram.ejs?a=b&c=d&e=f")
    match("url", "/ejsProgram.ejs")
    match("a", "b")
    match("c", "d")

    //
    //  Query string vars should not be turned into variables for GETs
    //
    http.get(HTTP + "/ejsProgram.ejs?var1=a+a&var2=b%20b&var3=c")
    match("url", "/ejsProgram.ejs")
    match("query", "var1=a+a&var2=b%20b&var3=c")
    match("var1", "a a")
    match("var2", "b b")
    match("var3", "c")

    //
    //  Post data should be turned into variables
    //
    http.form(HTTP + "/ejsProgram.ejs?var1=a+a&var2=b%20b&var3=c", 
        { name: "Peter", address: "777+Mulberry+Lane" })
    match("query", "var1=a+a&var2=b%20b&var3=c")
    match("var1", "a a")
    match("var2", "b b")
    match("var3", "c")
    match("name", "Peter")
    match("address", "777 Mulberry Lane")
}

function encoding() {
    http.get(HTTP + "/ejsProgram.ejs?var%201=value%201")
    match("query", "var%201=value%201")
    match("url", "/ejsProgram.ejs")
    match("var 1", "value 1")
}

function status() {
    http.get(HTTP + "/ejsProgram.ejs?code=711")
    assert(http.code == 711)
}

function location() {
    let http = new Http
    http.followRedirects = false
    http.get(HTTP + "/ejsProgram.ejs?redirect=http://www.redhat.com/")
    assert(http.code == 302)
    http.close()
}

function quoting() {
    http.get(HTTP + "/ejsProgram.ejs?a+b+c")
    match("query", "a+b+c")
    match("a b c", "")

    http.get(HTTP + "/ejsProgram.ejs?a=1&b=2&c=3")
    match("query", "a=1&b=2&c=3")
    match("a", "1")
    match("b", "2")
    match("c", "3")

    http.get(HTTP + "/ejsProgram.ejs?a%20a=1%201+b%20b=2%202")
    match("query", "a%20a=1%201+b%20b=2%202")
    match("a a", "1 1 b b=2 2")

    http.get(HTTP + "/ejsProgram.ejs?a%20a=1%201&b%20b=2%202")
    match("query", "a%20a=1%201&b%20b=2%202")
    match("a a", "1 1")
    match("b b", "2 2")

    http.get(HTTP + "/ejsProgram.ejs?a|b+c>d+e?f+g>h+i'j+k\"l+m%20n")
    contains("a|b c>d e?f g>h i'j k\"l m n")
    contains("query\": \"a|b+c>d+e?f+g>h+i\\'j+k\\\"l+m%20n")
}

basic()
forms()
alias()
query()
encoding()
status()
location()
quoting()

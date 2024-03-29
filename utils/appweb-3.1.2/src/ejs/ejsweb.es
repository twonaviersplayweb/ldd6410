#!/usr/bin/env ejs
/*
 *  ejsweb.es -- Ejscript web framework generator. This generates, compiles, cleans and packages Ejscript web applications. 
 *  For windows, this will be invoked via ejsweb.exe which is an Ejscript interpreter executable.
 *  For Appweb, it will be installed as ajsweb
 */

require ejs.db
require ejs.web

use default namespace public

class EjsWeb {

    private const DIR_PERMS: Number = 0775
    private const FILE_PERMS: Number = 0666
    private const RC: String = ".ejsrc"
    private const DefaultLayout: String = "views/layouts/default.ejs"
    private const NextMigration: String = ".ejs/nextMigration"

    private var database: String = "sqlite"
    private var applyMigration: Boolean = false
    private var appName: String
    private var binDir: Path
    private var command: String 
    private var config: Object
    private var db: Database
    private var dbPath: String
    private var debug: Boolean = false
    private var keep: Boolean = false
    private var layoutPage: String = DefaultLayout
    private var libDir: Path
    private var modDir: Path
    private var mode: String = "debug"
    private var overwrite: Boolean = false
    private var reverse: Boolean = false
    private var searchPath: String
    private var verbose: Number = 1
    private var compiler: String
    private var ejsweb: String

    function EjsWeb() {

        binDir = App.exeDir
        if (binDir == Config.BinDir) {
            libDir = Config.LibDir
            modDir = Config.ModDir
        } else {
            libDir = binDir.dirname.join("lib")
            modDir = binDir.dirname.join("modules")
            if (!exists(libDir)) {
                libDir = binDir.dirname.dirname.join("lib")
                modDir = binDir.dirname.dirname.join("modules")
                if (!exists(libDir)) {
                    if (exists(binDir.join("ejsweb")) && exists(binDir.join("ejsc"))) {
                        libDir = binDir
                        modDir = binDir
                    } else if (Config.Product != "appweb") {
                        //  TODO TEMP. Just to allow appweb IDE when there is no lib dir
                        throw "Can't find required lib directory: " + libDir
                    }
                }
            }
        }
        if (isApp()) {
            searchPath = App.dir.join("modules")
        } else {
            searchPath = App.dir.join("../modules")
        }
        App.searchPath = searchPath

        config = loadEcf("config/config.ecf", false)
        if (config) {
            mode = config.app.mode
        } else {
            config = {}
        }
        ejsweb = basename(App.args[0]).trimEnd(".exe").toString()
        if (ejsweb == "ejsweb") {
            compiler = "ejsc"
        } else {
            compiler = "ajsc"
        }

        loadConfigFile("config/compiler.ecf", "compiler", false)

        if (config.compiler == undefined) {
            config.compiler = {}
            config.compiler[mode] = {}
            config.compiler[mode].command = '"' + App.exeDir + '/' + compiler + '" --debug --web'
        } else {
            /*
             *  TODO - temp
             *  Be smart and map ajsc/ejsc to the right program for use by Appweb or Ejscript
             *  Just do for debug
             */
            if (ejsweb == "ejsweb") {
                config.compiler["debug"].command = config.compiler["debug"].command.replace(/ajsc/, "ejsc")
            } else {
                config.compiler["debug"].command = config.compiler["debug"].command.replace(/ejsc/, "ajsc")
            }
        }
    }


    /*
     *  Load the various default files
     */
    function loadDefaults(): Void {
        loadConfigFile(RC, "defaults") || loadConfigFile("~/" + RC, "defaults")
    }

    /*
     *  Parse args and invoke required commands
     */
    function parseArgs(args: Array): Boolean {
        let cmdArgs: Array

        for (let i: Number = 1; i < args.length; i++) {
            switch (args[i]) {
            case "-a":
            case "--apply":
                applyMigration = true
                break

            case "--database":
                database = args[++i]
                break

            case "--debug":
                debug = true
                break

            case "-k":
            case "--keep":
                keep = true
                break

            case "--layout":
                layoutPage = args[++i]
                break

            case "--overwrite":
                overwrite = true
                break

            case "-q":
            case "--quiet":
                verbose = 0
                break

            case "--reverse":
                reverse = true
                break

            case "--search":
                searchPath = App.searchPath = args[i++]
                break

            case "-v":
            case "--verbose":
                verbose++
                break

            default:
                if (args[i].startsWith("-")) {
                    usage()
                    return false
                }
                cmdArgs = args.slice(i)
                //  TODO BUG. Break not working
                i = 9999
                break
            }
        }

        if (cmdArgs == null || cmdArgs.length == 0) {
            usage()
            return false
        }

        let rest: Array = cmdArgs.slice(1)
        let cmd: String = cmdArgs[0].toLower()

        switch (cmd) {
        case "browse":
            checkApp(cmd, rest)
            browse(rest)
            break

        case "clean":
            clean(rest)
            break

        case "compile":
            if (isApp()) {
                checkApp(cmd, rest);
            }
            compile(rest)
            break

        case "console":
            checkApp(cmd, rest)
            console(rest)
            break

        case "deploy":
            checkApp(cmd, rest)
            deploy(rest)
            break

        case "import":
            checkApp(cmd, rest)
            import(rest)
            break

        case "install":
            checkApp(cmd, rest)
            install(rest)
            break

        case "generate":
            checkApp(cmd, rest)
            generate(rest)
            break

        case "migrate":
            checkApp(cmd, rest)
            migrate(rest)
            break

        case "run":
            checkApp(cmd, rest)
            run(rest)
            break

        default:
            rest = cmdArgs
            compile(rest)
            break
        }

        if (applyMigration) {
            migrate()
        }

        return true
    }

    function usage(): Void {
        print("\nUsage: " + ejsweb + " [options] [commands] ...\n" +
            "  Options:\n" + 
            "    --apply                      # Apply migrations\n" + 
            "    --database [sqlite | mysql]  # Sqlite only currently supported adapter\n" + 
            "    --keep\n" + 
            "    --layout layoutPage\n" + 
            "    --reverse                    # Reverse generated migrations\n" + 
            "    --overwrite\n" + 
            "    --quiet\n" + 
            "    --verbose\n")

        let pre = "    " + ejsweb + " "
        print("  Commands:\n" +
            pre + "clean\n" +
            pre + "compile [all | app | controller names | view names]\n" +
            pre + "compile path/name.ejs ...\n" +
            pre + "generate app name\n" + 
            pre + "generate controller name [action [, action] ...]\n" + 
            pre + "generate migration description model [field:type [, field:type]...]\n" +
            pre + "generate model name [field:type [, field:type]...]\n" +
            pre + "generate scaffold model [field:type [, field:type]...\n" +
            pre + "import\n" +
            pre + "migrate [forward|backward|NNN]\n" +
            pre + "run" +
            "")

        /*
         *  TODO
            pre + "dbconsole \n" +             # sqlite
            pre + "console \n" +               # with ejs.db, ejs.web and app loaded (all models, all controllers)
            pre + "generate package\n" +
            pre + "install (not implemented yet)\n" +
            pre + "uninstall (not implemented yet)\n" +
            pre + "mode [development | test | production] (not implemented yet)\n" +
            pre + "deploy path.zip (not implemented yet)\n" +
        */
        App.exit(1)
    }

    function clean(args: Array): Void {
        let files: Array = glob("controllers", /\.mod$/) + glob("views", /\.mod$/) + glob("models", /\.mod$/) + [ "App.mod"]
        trace("[CLEAN]", files)
        for each (f in files) {
            rm(f)
        }
    }

    //  TODO - refactor and reorder this. Perhaps move compile, generate, migrate etc into separate files

    function compile(args: Array): Void {
        var files: Array

        if (args.length == 0) {
            if (isApp()) {
                args.append("everything")
            } else {
                args.append(".")
            }
        }
        let kind: String = args[0].toLower()
        let rest: Array = args.slice(1)

        let ejspat = /\.ejs$/
        let pat = /\.es$/

        switch (kind) {
        case "everything":
            /*
             *  Build all items but NOT as one module
             */
            buildApp()
            for each (name in glob("controllers", pat)) {
                buildController(name)
            }
            files = glob("views", ejspat)
            for each (name in files) {
                buildView(name)
            }
            files = glob("web", ejspat)
            layoutPage = undefined;
            for each (name in files) {
                buildWebPage(name)
            }
            break

        case "all":
            /*
             *  Build entire app as one module
             */
            let saveVerbose = verbose
            let saveKeep = keep
            verbose = 0
            buildApp()
            files = glob("config", pat) + glob("src", pat) + glob("controllers", pat) + glob("models", pat)
            let viewFiles = glob("views", ejspat)
            esPages = []
            for each (name in viewFiles) {
                if (!name.toString().contains(/^views\/layouts\//)) {
                    esPages.append(buildWebPage(name, false, true))
                }
            }
            let webFiles = glob("web", ejspat)
            layoutPage = undefined
            for each (name in webFiles) {
                esPages.append(buildWebPage(name, false))
            }
            files += esPages
            keep = saveKeep
            verbose = saveVerbose
            buildFiles("App", files)
            if (!keep) {
                for each (name in esPages) {
                    rm(name)
                }
            }
            break

        case "app":
            /*
             *  Build app portions. This includes all config, src, models and BaseController
             */
            buildApp()
            break

        case "controller":
        case "controllers":
            /*
             *  Build controllers
             */
            if (rest.length == 0) {
                for each (name in glob("controllers", pat)) {
                    buildController(name)
                }
            } else {
                for each (name in rest) {
                    buildController(name)
                }
            }
            break

        case "model":
        case "models":
            throw "WARNING: models must be built with the app. Use \"" + ejsweb + " compile app\""
            /*
             *  Build models
             */
            if (rest.length == 0) {
                for each (name in glob("models", pat)) {
                    buildModel(name)
                }
            } else {
                for each (name in rest) {
                    buildModel(name)
                }
            }
            break

        case "view":
        case "views":
            /*
             *  Comile views
             */
            if (rest.length == 0) {
                for each (view in glob("views", ejspat)) {
                    buildView(view)
                }
            } else {
                for each (view in rest) {
                    buildView(view)
                }
            }
            break

        default:
            for each (f in args) {
                compileItem(Path(f).relative)
            }
        }
    }

    function compileItem(file: Path) {
        if (file.isDir) {
            for each (f in file.files(true)) {
                compileItem(f)
            }
        } else {
            if (file.extension == '.ejs') {
                layoutPage = undefined
                buildWebPage(file.toString())
            } else if (file.extension == '.es') {
                build(file.toString())
            } else {
                error(ejsweb + ": Can't compile " + file)
            }
        }
    }

    function console(args: Array): Void {
        // cmd = 'ejs --use "' + appName + '"'
        let cmd = "ejs"
        //  TODO - this won't work without stdin
        System.run(cmd)
    }

    function buildController(file: String) {
        let testFile = "controllers/" + file.toPascal() + ".es"
        if (exists(testFile)) {
            file = testFile
        }
        if (!file.startsWith("controllers/")) {
            throw "File \"" + file + "\" is not a controller"
        }
        if (!exists(file)) {
            throw "Controller : \"" + file + "\" does not exist"   
        }
        if (file == "controllers/Base.es") {
            return
        }
        build(file)
    }

    function buildModel(file: String) {
        let testFile = "models/" + file.toPascal() + ".es"
        if (exists(testFile)) {
            file = testFile
        }
        if (!file.startsWith("models/")) {
            throw "File \"" + file + "\" is not a controller"
        }
        if (!exists(file)) {
            throw "Model : \"" + file + "\" does not exist"   
        }
        build(file)
    }

    function buildView(file: String) {
        if (file.contains(/^views\/layouts\//)) {
            /*
             *  Skip layouts
             */
            return
        }
        if (!file.startsWith("views/")) {
            throw "File \"" + file + " \" is not a view. Path should be \"views/CONTROLLER/VIEW.ejs\""
        }
        buildWebPage(file, true, true)
    }

    function buildWebPage(file: String, compile: Boolean = true, isView: Boolean = false): String {
        let ext = extension(file)
        if (ext == "") {
            file += ".ejs"
        } else if (ext != ".ejs") {
            throw "File is not an Ejscript web file: " + file
        }
        if (!exists(file)) {
            if (ext) {
                throw "Can't find ejs page: " + file
            } else {
                throw "Can't find view file: " + file
            }
        }

        let sansExt = file.replace(/.ejs$/,"")
        let controller: String
        let controllerMod: String
        let controllerSource: String
        let controllerPrefix: String
        let viewName: String

        //  TODO - refactor this logic
        if (isView) {
            controller = getNthSegment(sansExt, 1).toPascal()
            viewName = basename(sansExt)

        } else {
            // viewName = sansExt.split(/(\\|\/)+/g).slice(1).join("_")
            viewName = sansExt.replace(/(\\|\/)+/g, "_")
            if (exists("config/compiler.ecf")) {
                controllerPrefix = "Base" + "_"
            } else {
                controllerPrefix = "_Solo" + "_"
            }
        }

        /*
         *  Ensure the corresponding controller (if there is one) is built first
         */
        controllerMod = "controllers/" + controller + ".mod"
        controllerSource = "controllers/" + controller + ".es"

        if (isView && exists(controllerSource)) {
            if (!exists(controllerMod)) {
                build("controllers/" + controller + ".es")
            }
            controllerPrefix = controller + "_"
        }

        /*
         *  Parse the ejs file and create an ".es" equivalent
         */
        if (verbose > 1) {
            trace("[PARSE]", file)
        }
        let ep: EjsParser = new EjsParser
        results = Templates.ViewHeader + ep.parse(file, App.dir, layoutPage) + Templates.ViewFooter
        results = results.replace(/\${CONTROLLER}/g, controllerPrefix)
        results = results.replace(/\${VIEW}/g, viewName)

        let esfile = sansExt + ".es"

        try {
            Path(esfile).write(results)
        } catch (e) {
            throw "Can't write module file: " + esfile + ". Ensure directory is writable."
        }

        if (compile) {
            let out = sansExt + ".tmod"
            let cmd: String = getCompilerPath()
            if (exists(controllerMod)) {
                cmd += " --out " + out + " --search \"" + searchPath + "\" App.mod " + controllerMod + " " + esfile
            } else if (appName) {
                cmd += " --out " + out + " --search \"" + searchPath + "\" App.mod " + esfile
            } else {
                cmd += " --out " + out + " --search \"" + searchPath + "\" " + esfile
            }

            if (verbose > 1) {
                trace("[BUILD]", cmd)
            } else {
                trace("[BUILD]", file)
            }
            command(cmd)

            if (!exists(out)) {
                throw "Compilation failed for " + out + "\n" + results
            }
            if (!keep) {
                rm(esfile)
            }
            mv(out, sansExt + ".mod")
        }
        return esfile
    }

    function getCompilerPath(): String {
        let cmd = config.compiler[mode].command
        if (cmd.trim('"').match(/^\/|^[a-zA-Z]:\//)) {
            return cmd
        }
        parts = cmd.split(" ")
        cmd = '"' + App.exeDir + '/' + parts[0] + '" ' + parts.slice(1).join(" ")
        return cmd
    }

    /*
     *  Build the entire app into a single mod file. 
     */
    function buildFiles(name: String, files: Array) {
        let out = name + ".tmod"
        let cmd = getCompilerPath() + " --out " + out + " --search \"" + searchPath + "\" " + files.join(" ")
        if (verbose > 1) {
            trace("[BUILD]", cmd)
        } else {
            trace("[BUILD]", files.join(" "))
        }
        let results = command(cmd)
        if (!exists(out)) {
            throw "Compilation failed for " + out + "\n" + results
        }
        mv(out, name + ".mod")
    }

    function buildApp(): Void {
        let pat = /\.es$/
        buildFiles("App", glob("config", pat) + glob("src", pat) + glob("models", pat) + glob("controllers", /Base.es$/))
    }

    /*
     *  Build a single file. Used for controllers and models.
     */
    function build(files: String) {
        let name = files.replace(/.es$/,"")
        let out = name + ".tmod"

        let cmd: String
        if (appName) {
            cmd = getCompilerPath() + " --out " + out + " --search \"" + searchPath + "\" App.mod " + files
        } else {
            cmd = getCompilerPath() + " --out " + out + " --search \"" + searchPath + "\" " + files
        }
        if (verbose > 1) {
            trace("[BUILD]", cmd)
        } else {
            trace("[BUILD]", files)
        }
        let results = command(cmd)
        if (!exists(out)) {
            throw "Compilation failed for " + out + "\n" + results
        }
        mv(out, name + ".mod")
    }

    function browse(args: Array): Void {
        throw("No yet supported")
        let cmd = config.app.webserver

        if (!cmd.match(/^\/|^[a-zA-Z]:\//)) {
            cmd = cmd.trim('"').replace(/^[^ ]+/, App.exeDir + "/" + "$&")
        }
        trace("[RUN]", cmd)
        System.run(cmd)
    }

    function deploy(args: Array): Void {
    }

    function import(args: Array): Void {
        // print("libDir " + libDir)
        // print("binDir " + binDir)
        // print("modDir " + modDir)

        overwrite = true
        let exe = ""
        let lib = ""
        switch (Config.OS) {
        case "WIN":
            exe = ".exe"
            lib = ".dll"
            break
        case "MACOSX":
            lib = ".dylib"
            break
        default:
            lib = ".so"
            break
        }

        //  TODO - import must support ajs alternatives
        binFiles = [ "ejs", "ejsc", "ejsweb", "ejswebserver" ]
        extFiles = [ "libcrypto", "libssl", "libmprssl" ]
        confFiles = [ "ejswebserver.conf" ]

        if (exists(modDir.join("ejs.db.mod"))) {
            modFiles = [ "ejs.mod", "ejs.db.mod", "ejs.db.sqlite.mod", "ejs.web.mod", "ejsweb.mod" ]
            modLibFiles = [ "ejs.db.sqlite", "ejs.web" ]
            libFiles = [ "libpcre", "libec", "libmpr", "libsqlite3", "libejs" ]
        } else {
            //  Static
            modFiles = [ "ejs.mod", "ejsweb.mod" ]
            modLibFiles = [ ]
            libFiles = [ ]
            binFiles = [ "ejsc", "ejsweb", "ejswebserver" ]
        }

        for each (file in modFiles) {
            src = modDir.join(file)
            dest = Path("bin").join(file)
            if (!exists(src)) {
                error("WARNING: Can't find: " + file + " Continuing ...")
            }
            copyFile(src, dest, "Import")
            chmod(dest, 0644)
        }
        for each (file in modLibFiles) {
            src = modDir.join(file).toString() + lib
            dest = Path("bin").join(file).toString() + lib
            if (!exists(src)) {
                error("WARNING: Can't find: " + file + " Continuing ...")
            }
            copyFile(src, dest, "Import")
            chmod(dest, 0644)
        }
        for each (file in libFiles) {
            dest = Path("bin").join(file).joinExt(lib)
            src = libDir.join(file).joinExt(lib)
            if (!exists(src)) {
                error("WARNING: Can't find: " + file + " Continuing ...")
            }
            copyFile(src, dest, "Import")
            chmod(dest, 0755)
        }
        for each (file in binFiles) {
            dest = Path("bin").join(file).joinExt(exe)
            src = binDir.join(file).joinExt(exe)
            if (!exists(src)) {
                error("WARNING: Can't find: " + file + " Continuing ...")
            }
            copyFile(src, dest, "Import")
            chmod(dest, 0755)
        }
        for each (file in extFiles) {
            dest = Path("bin").join(file).joinExt(lib)
            src = binDir.join(file).joinExt(lib)
            if (exists(file)) {
                copyFile(src, dest, "Import")
                chmod(dest, 0755)
            }
        }

        overwrite = false
        for each (file in confFiles) {
            dest = Path("bin").join(file)
            src = libDir.join(file)
            if (!exists(src)) {
                error("WARNING: Can't find: " + file + " Continuing ...")
            }
            copyFile(src, dest, "Import")
            chmod(dest, 0644)
        }
        /*
         *  Update the DocumentRoot in bin/ejswebserver.conf
         */
        path = new Path("bin/ejswebserver.conf")
        data = path.readString()
        path.write(data.replace(/DocumentRoot ".*"/, 'DocumentRoot "../web"'))
    }

    function install(args: Array): Void {
    }

    function uninstall(args: Array): Void {
    }

    function generate(args: Array): Void {
        if (args.length == 0) {
            args.append("all")
        }

        let kind: String = args[0].toLower()
        let rest: Array = args.slice(1)

        if (rest.length == 0) {
            usage()
            return
        }

        switch (kind) {
        case "app":
            generateApp(rest)
            break

        case "controller":
            generateController(rest)
            break

        case "migration":
            generateMigration(rest)
            break

        case "model":
            generateModel(rest, "Create Model " + rest[0].toPascal())
            break

        case "scaffold":
            generateScaffold(rest)
            break

        default:
            usage()
            return
        }
    }

    /*
     *  ejsweb migrate              # Apply all migrations
     *  ejsweb migrate NNN          # Intelliegently set to a specific migration
     *  ejsweb migrate forward      # Migrate forward one
     *  ejsweb migrate backward     # Migrate backward one
     */
    function migrate(args: Array = null): Void {
        let files = Path("db/migrations").files().sort()
        let onlyOne = false
        let backward = false
        let targetSeq = null
        let id = null

        /*
         *  Load the models
         */
        if (!exists("App.mod")) {
            buildApp()
        }
        load("App.mod")

        if (overwrite) {
            rm(dbPath)
            generateDatabase()
        }
        let migrations = _EjsMigration.findAll()
        let lastMigration = migrations.slice(-1)

        if (args && args.length > 0) {
            cmd = args.slice(0).toString().toLower()
        } else {
            cmd = ""
        }
        if (cmd == "forward" || cmd == "forw") {
            onlyOne = true

        } else if (cmd == "backward" || cmd == "back") {
            onlyOne = true
            backward = true

        } else if (cmd != "") {
            /* cmd may be a pure sequence number or a filename */
            targetSeq = cmd;
            let found = false
            for each (f in files) {
                let base = basename(f).toLower()
                if (basename(targetSeq) == base) {
                    targetSeq = base.replace(/^([0-9]*)_.*es/, "$1")
                    found = true
                } else {
                    let seq = base.replace(/^([0-9]*)_.*es/, "$1")
                    if (seq == targetSeq) {
                        found = true
                    }
                }
            }
            if (! found) {
                throw "Can't find target migration: " + targetSeq
            }
            if (lastMigration && targetSeq < lastMigration[0].version) {
                backward = true
            }
        }

        if (backward) {
            files = files.reverse()
        }

        for each (f in files) {
            let base = basename(f).toString()
            if (!base.match(/^([0-9]+).*es/)) {
                continue
            }
            let seq = base.replace(/^([0-9]*)_.*es/, "$1")
            if (seq == "") {
                continue
            }
            let found = false
            for each (appliedMigration in migrations) {
                if (appliedMigration["version"] == seq) {
                    found = true
                    id = appliedMigration["id"]
                }
            }
            if (backward) {
                found = !found
                if (targetSeq && targetSeq == seq) {
                    return
                }
            }

            if (!found) {
                try { delete Migration; } catch {}
                load(f)
                if (backward) {
                    trace("[MIGRATE]", "Reverse " + base)
                    new Migration().backward(db)
                } else {
                    trace("[MIGRATE]", "Apply " + base)
                    new Migration().forward(db)
                }
                if (backward) {
                    _EjsMigration.remove(id)
                } else {
                    migration = new _EjsMigration
                    migration["version"] = seq.toString()
                    migration.save()
                }
                if (onlyOne) {
                    return
                }
            }
            if (!backward && targetSeq && targetSeq == seq) {
                return
            }
        }
        if (onlyOne) {
            if (backward) {
                trace("[OMIT]", "All migrations reversed")
            } else {
                trace("[OMIT]", "All migrations applied")
            }
        }
    }

    function run(args: Array): Void {
        let cmd = config.app.webserver

        /*
         *  Expand ${HOME} to be /usr/lib/XXX or in ejs: ./src/appweb or in appweb: ./src/server 
         */
        if (App.exeDir == Config.BinDir) {
            cmd = cmd.replace(/\${HOME}/g, libDir)
        } else if (App.exeDir == libDir) {
            cmd = cmd.replace(/\${HOME}/g, libDir)
        } else {
            top = findTop(App.exeDir)
            if (Config.Product == "ejs") {
                cmd = cmd.replace(/\${HOME}/g, top.join("src/appweb"))
            } else {
                //  TODO - should require products that import Ejscript to create a "hosting" directory
                cmd = cmd.replace(/\${HOME}/g, top.join("/src/server"))
            }
        }
        if (Config.Product == "appweb") {
            cmd = cmd.replace(/ejswebserver/g, "appweb")
        }

        //  TODO - this should really just use PATH
        /*
         *  Handle quotes around the program name. Because we try to convert to an absolute path, we need
         *  to remove quotes around a program name that has no spaces. If the program name has spaces or starts
         *  with quotes, we really can't convert to an absolute path easily.
         */
        argv = cmd.split(" ")
        if (argv[0].startsWith('"') && argv[0].endsWith('"')) {
            argv[0] = argv[0].trim('"')
            cmd = argv.join(" ")
        }
        if (!cmd.match(/^\/|^[a-zA-Z]:\//) && !cmd.startsWith('"')) {
            cmd = cmd.replace(/^[^ ]+/, App.exeDir.name + "/" + "$&")
        }

        trace("[RUN]", cmd)
        System.runx(cmd)
    }

    function findTop(dir: Path) {
        do {
            if (exists(dir.join("buildConfig.h"))) {
                return dir
            }
            prev = dir
            dir = dir.parent
        } while (!dir.same(prev))
        throw "Can't find XXX buildConfig.h in local source tree"
    }

    /*
     *  Generate an application.
     *
     *  ejsweb generate app appName
     */
    function generateApp(args: Array): Void {

        appName = args[0].toLower()
        let f: File = new Path(appName)

        makeDir(appName)
        App.chdir(appName)
        makeDir(".tmp")
        makeDir(".ejs")
        makeDir("bin")
        makeDir("config")
        makeDir("controllers")
        makeDir("db")
        makeDir("db/migrations")
        makeDir("doc")
        makeDir("logs")
        makeDir("models")
        makeDir("messages")
        makeDir("test")
        makeDir("src")
        makeDir("utils")
        makeDir("views")
        makeDir("views/layouts")
        makeDir("web")
        makeDir("web/default")
        makeDir("web/images")
        makeDir("web/themes")

        generateAppSrc()
        generateConfig()
        generateLayouts()
        generatePages()
        generateBaseController()
        generateReadme()
        generateDatabase()

        buildFiles("App", ["controllers/Base.es"])
        App.chdir("..")

        if (verbose) {
            print("\nChange directory into your application directory: " + appName)
            print("Then run the web server via: \"" + ejsweb + " run\"")
            print("and point your browser at: http://localhost:4000/ to view your app.")
        }
    }

    function generateConfig(): Void {
        let data = Templates.Config.replace(/\${NAME}/g, appName)
        data = data.replace(/\${PATH}/g, App.dir)
        if (Config.Product == "ejs") {
            data = data.replace(/\${WEBSERVER}/g, "ejswebserver")
        } else {
            data = data.replace(/\${WEBSERVER}/g, "appweb")
        }
        data = data.replace(/\${BINDIR}/g, binDir)
        if (Config.OS == "WIN") {
            data = data.replace(/\${HOME}/g, binDir)
        } else {
            data = data.replace(/\${HOME}/g, libDir)
        }
        makeConfigFile("config/config.ecf", data)
        makeConfigFile("config/compiler.ecf", Templates.Compiler.replace(/\${COMPILER}/g, compiler))
        makeConfigFile("config/database.ecf", Templates.Database)
        makeConfigFile("config/view.ecf", Templates.View)
    }

    function generateLayouts(): Void {
        let data = Templates.DefaultLayout.replace(/\${NAME}/g, appName.toPascal())
        makeFile("views/layouts/default.ejs", data, "Layout")
    }

    function generatePages(): Void {
        path = libDir.join("default-web")
        if (!exists(path)) {
            throw "Can't find default-web at " + path
        }
        for each (f in glob(path, /.*/)) {
            copyFile(f, "web" + f.name.slice(path.length), "Web File")
        }
    }

    function generateBaseController(): Void {
        let path = "controllers/Base.es"
        let data = Templates.BaseController.replace(/\${NAME}/g, appName)
        makeFile(path, data, "BaseController")
    }

    function generateAppSrc(): Void {
        let data: String = Templates.AppSrc
        makeFile("src/App.es", data, "App")
    }

    function generateReadme(): Void {
        let data: String = Templates.Readme.replace(/\${NAME}/g, appName.toPascal())
        makeFile("README", data, "README")
    }

    function generateDatabase(): Void {
        db = new Database(database, "db/" + appName + ".sdb")
        if (debug) {
            db.trace(true)
        }
        db.createTable("_EjsMigrations", ["version:string"])
    }

    /*
     *  ejsweb generate controller name [action ...]
     */
    function generateController(args: Array): Void {
        let name: String = args[0].toPascal()
        let actions = args.slice(1)
        let path: String = "controllers/" + name + ".es"
        let data: String = Templates.Controller.replace(/\${NAME}/g, name)
        data = data.replace(/\${APP}/g, appName)

        if (actions.length == 0) {
            actions.append("index")
        }
        for each (action in actions) {
            let actionData = Templates.Action.replace(/\${NAME}/g, action)
            data = data.replace(/NEXT_ACTION/, actionData + "NEXT_ACTION")
        }
        data = data.replace(/NEXT_ACTION/, "")
        makeFile(path, data, "Controller")
    }

    function createMigrationCode(model: String, forward: String, backward: String, comment: String) {
        data = Templates.Migration
        data = data.replace(/\${COMMENT}/g, comment)
        data = data.replace(/\${FORWARD}/g, forward)
        data = data.replace(/\${BACKWARD}/g, backward)

        seq = (new Date()).format("%Y%m%d%H%M%S")
        fileComment = comment.replace(/[    ]+/g, "_")
        path = "db/migrations/" + seq + "_" + fileComment + ".es"
        if (exists(path)) {
            throw "Migration " + path + " already exists. Try again later."
        }
        makeFile(path, data, "Migration")
    }

    function validateAttributes(attributes: Array): Void {
        for each (attribute in attributes) {
            column = attribute.split(":")[0]
            datatype = attribute.split(":")[1]
            if (db.dataTypeToSqlType(datatype) == undefined) {
                throw "Unsupported data type: \"" + datatype + "\" for column \"" + column + "\""
            }
        }
    }

    function createMigration(model: String, attributes: Array, comment: String, tableExists: Boolean): Void {

        let tableName = plural(model).toPascal();

        let forward = ''
        let backward = ''

        if (attributes && attributes.length > 0) {
            validateAttributes(attributes)
            if (!tableExists) {
                forward = '        db.createTable("' + tableName + '", ["' + attributes.join('", "') + '"])'
                backward = '        db.destroyTable("' + tableName + '")'

            } else {
                forward = ""
                for each (col in attributes)  {
                    spec = col.split(":")
                    forward += '        db.addColumn("' + tableName + '", "' + spec[0] + '", "' + spec[1] + '")\n'
                }
                backward = '        db.removeColumns("' + tableName + '", ['
                for each (col in attributes) {
                    backward += '"' + col.split(":")[0] + '", '
                }
                backward += '])'
            }

        } else {
            if (reverse) {
                forward = '        db.destroyTable("' + tableName + '")'
            }
        }
        if (reverse) {
            createMigrationCode(model, backward, forward, comment)
        } else {
            createMigrationCode(model, forward, backward, comment)
        }
    }

    /*
     *  ejsweb generate migration description model [field:type ...]
     */
    function generateMigration(args: Array): Void {
        if (args.length < 2) {
            usage()
        }
        comment = args[0]
        model = args[1]
        createMigration(model, args.slice(2), comment, true)
    }

    /*
     *  ejsweb generate model name [field:type ...]
     */
    function generateModel(args: Array, comment: String): Void {
        let model: String = args[0].toPascal()
        if (model.endsWith("s")) {
            error("WARNING: Models should typically be singluar not plural. Continuing ...")
        }
        let path = "models/" + model + ".es"

        if (exists(path) && !overwrite) {
            traceFile(path, "[EXISTS] Migration (model already exists)")
        } else {
            createMigration(model, args.slice(1), comment, false)
        }

        let data = Templates.Model.replace(/\${NAME}/g, model)
        makeFile(path, data, "Model")
    }

    /*
     *  ejsweb generate scaffold model [field:type ...]
     */
    function generateScaffold(args: Array): Void {
        let model = args[0]
        if (model.match(/[a-zA-Z_]*/) != model) {
            abort("Bad model name " + model);
        }
        let controller = model.toPascal()
        let attributes = args.slice(2)

        makeDir("views/" + controller)
        generateModel(args, "Create Scaffold " + model)
        generateScaffoldController(controller, model)
        generateScaffoldViews(controller, model)
        buildApp()
        if (!applyMigration /* && !verbose */) {
            print("\nDon't forget to apply the database migration. Run: \"" + ejsweb + " migrate\"")
        }
    }

    /*
     *  Create a controller with scaffolding. Usage: controllerName [actions ...]
     */
    function generateScaffoldController(controller: String, model: String, extraActions: Array = null): Void {
        let name = controller.toPascal()
        let path = "controllers/" + name + ".es"

        let stndActions: Array = [ "index", "list", "create", "edit", "update", "destroy" ]
        let views: Array = [ "list", "edit" ]
        let actions: Array = []

        if (extraActions) {
            for each (action in extraActions) {
                if (! stndActions.contains(action)) {
                    actions.append(action.toCamel())
                }
            }
        }

        let data: String = Templates.ScaffoldController.replace(/\${APP}/g, appName.toPascal())
        data = data.replace(/\${NAME}/g, name)
        data = data.replace(/\${MODEL}/g, model.toPascal())
        data = data.replace(/\${LOWER_MODEL}/g, model.toLower())

        for each (action in actions) {
            let actionData = Templates.Action.replace(/\${NAME}/g, action)
            data = data.replace(/NEXT_ACTION/, actionData + "NEXT_ACTION")
        }
        data = data.replace(/NEXT_ACTION/, "")

        makeFile(path, data, "Controller")
    }

    /*
     *  Create a scaffold views.  Usage: controllerName [actions ...]
     */
    function generateScaffoldViews(controller: String, model: String, extraActions: Array = null): Void {

        let stndActions: Array = [ "index", "list", "create", "edit", "update", "destroy" ]
        let views: Array = [ "list", "edit" ]
        let actions: Array = stndActions.clone()

        if (extraActions) {
            for each (action in extraActions) {
                if (! stndActions.contains(action)) {
                    views.append(action.toCamel())
                }
            }
        }
        let data: String

        model = model.toPascal()

        for each (view in views) {
            switch (view) {
            case "edit":
                data = Templates.ScaffoldEditView.replace(/\${MODEL}/g, model)
                data = data.replace(/\${LOWER_MODEL}/g, model.toLower())
                break
            case "list":
                data = Templates.ScaffoldListView.replace(/\${MODEL}/g, model)
                break
            default:
                data = Templates.ScaffoldView.replace(/\${MODEL}/g, model)
                data = data.replace(/\${LOWER_MODEL}/g, model.toLower())
                data = data.replace(/\${CONTROLLER}/g, controller)
                data = data.replace(/\${VIEW}/g, view)
                break
            }
            let path: String = "views/" + controller + "/" + view + ".ejs"
            makeFile(path, data, "View")
        }
    }

    //  TODO - should cache the result
    function isApp(fatal: Boolean = false): Boolean {
        let dirs: Array = [ "config", "controllers", "views"  ]
        for each (d in dirs) {
            if (! isDir(d)) {
                if (fatal) {
                    throw "Can't find \"" + d + "\" directory. Run from inside the application directory"
                }
                return false
            }
        }

        let files: Array = [ "config/compiler.ecf", "config/config.ecf", "config/database.ecf", "config/view.ecf" ]
        for each (f in files) {
            if (! exists(f)) {
                if (fatal) {
                    throw "Can't find \"" + f + "\" Run from inside the application directory\n" +
                          "Use " + ejsweb + " generate app NAME to create a new Ejscript web application"
                }
                return false
            }
        }
        return true
    }

    function checkApp(cmd: String, rest: Array): Void {
        if (cmd == "generate") {
            let what = rest[0]
            if (rest[0] == "app") {
                return
            }
            if (what != "app" && what != "controller" && what != "migration" && what != "model" && what != "scaffold") {
                usage()
                App.exit()
            }
        }
        if (!isApp(true)) {
            return
        }
        appName = App.dir.basename.toString().toLower()
        dbPath = "db/" + appName + ".sdb"

        if (!exists(dbPath)) {
            generateDatabase()
        }

        //  TODO OPT -- should not be creating a database instance
        db = Database.defaultDatabase = new Database(database, "db/" + appName + ".sdb")
        if (debug) {
            db.trace(true)
            _EjsMigration.trace(true)
        }
    }

    function loadConfigFile(file: String, objName: String, mandatory: Boolean = false): Boolean {
        let settings: Object = loadEcf(file, mandatory)
        if (settings == null) {
            return false
        }
        let obj = config[objName] = {}
        for (key in settings) {
            obj[key] = settings[key]
        }
        return true
    }

    function loadEcf(path: String, mandatory: Boolean = false): Object {
        if (!exists(path)) {
            if (mandatory) {
                throw new IOError("Can't open required configuration file: " + path)
            } else {
                return null
            }
        }
        try {
            let data = "{ " + Path(path).readString() + " }"
            return deserialize(data)
        } catch (e: Error) {
            throw new IOError("Can't load " + path + " " + e)
        }
    }

    /*
     *  Make an ECF file that lives under ./config
     */
    function makeConfigFile(path: String, data: String): Void {
        if (exists(path) && !overwrite) {
            return
        }
        data = data.replace(/\${NAME}/g, appName)
        makeFile(path, data, "Config File")
    }

    function makeFile(path: String, data: String, msg: String): Void {

        let p: Path = new Path(path)
        if (p.exists && !overwrite) {
            traceFile(path, "[EXISTS] " + msg)
            return
        }

        if (! p.exists) {
            traceFile(path, "[CREATED] " + msg)
        } else {
            traceFile(path, "[OVERWRITTEN] " + msg)
        }

        let f: File = new File(path)
        f.open("w")
        f.write(data)
        f.close()
    }

    function makeDir(path: String): Void {
        if (isDir(path)) {
            return
        }
        trace("[CREATED] " + "Directory", path)
        mkdir(path, DIR_PERMS)
    }

    function copyFile(from: String, to: String, msg: String) {

        let p: Path = new Path(to)
        if (p.exists && !overwrite) {
            traceFile(to, "[EXISTS] " + msg)
            return
        }

        if (! p.exists) {
            traceFile(to, "[CREATED] " + msg)
        } else {
            traceFile(to, "[OVERWRITTEN] " + msg)
        }
        makeDir(p.dirname)
        cp(from, to)
    }

    /*
     *  Find all files matching the pattern 
     *  TODO - better to put pattern and recurse first and then can take a list of paths. 
     */
    function glob(path: Object, pattern: RegExp, recurse: Boolean = true): Array {
        let result: Array = new Array
        if (isDir(path)) {
            if (recurse) {
                for each (f in ls(path, true)) {
                    let got: Array = glob(f, pattern)
                    for each (i in got) {
                        result.append(i)
                    }
                }
            }

        } else {
            if (path.toString().match(pattern)) {
                result.append(new Path(path))
            }
        }
        return result
    }

    //  TODO - this functionality should be pushed into Path.files(/regexp/)
    function globSubdirs(path: String): Array {
        let result: Array = new Array
        for each (f in ls(path, true)) {
            if (isDir(f)) {
                result.append(f)
            }
        }
        return result
    }

    function getNthSegment(path: String, nth: Number) {
        let segments: Array = path.split(/(\\|\/)+/g)
        for (let i: Number = segments.length - 1; i >= 0; i--) {
            if (segments[0] == ".") {
                segments.remove(i, i)
            }
        }
        return segments[nth]
    }

    function command(cmd: String): String {
        let results
        try {
            results = System.run(cmd)
        } 
        catch (e) {
            msg = "Compilation failure, for " + cmd + "\n\n" +
                e.toString().replace(/Error Exception: Command failed: Status code [0-9]*.\n/, "")
            throw msg
        }
        return results
    }

    function traceFile(path: String, msg: String): Void {
        //  TODO - string method to add quotes would be useful
        trace(msg, '"' + path + '"')
    }

    function trace(tag: String, ...args): Void {
        if (verbose) {
            print("  " + tag + ": " + args.join(" "))
        }
    }

    //  TODO - share with rest of framework
    function plural(word: String): String {
        return word + "s"
    }

    function singular(word: String) {
        //  TODO
    }

    function abort(msg: String): Void {
        error(ejsweb + ": " + msg)
        usage()
        throw "Exiting"
    }
}


dynamic class _EjsMigration implements Record {
    function _EjsMigration(fields: Object = null) {
        constructor(fields)
    }
}


/*
 *  Templates for various files
 */
class Templates {
    
    /*
     ***************** config/config.ecf template ***********************
     */
    public static const Config =
'
app: {
    mode: "debug",
    webserver: \'"${BINDIR}/${WEBSERVER}" --home "${HOME}" --ejs "/:${PATH}/" --log stdout:2\',
},
'


    /*
     ***************** config/compiler.ecf template ***********************
     */
    public static const Compiler = 
"
debug: {
    command: '${COMPILER} --lang fixed --debug --optimize 9 --web ',
},

test: {
    command: '${COMPILER} --lang fixed --debug --optimize 9 --web ',
},

production: {
    command: '${COMPILER} --lang fixed --optimize 9 --web ',
},
"


    /*
     ***************** config/database.ecf template ***********************
     */
    public static const Database = 
'
debug: {
    adapter: "sqlite3",
    database: "db/${NAME}.sdb",
    username: "",
    password: "",
    timeout: 5000,
    trace: true,
},

test: {
    adapter: "sqlite3",
    database: "db/${NAME}.sdb",
    username: "",
    password: "",
    timeout: 5000,
    trace: false,
},

production: {
    adapter: "sqlite3",
    database: "db/${NAME}.sdb",
    username: "",
    password: "",
    timeout: 5000,
    trace: false,
},
'


    /*
     ***************** config/view.ecf template ***********************
     */
    public static const View = 
'
connectors: {
    table: "html",
    chart: "google",
    rest: "html",
},

'


    /*
     *****************  BaseController template ***********************
     */
    public static const BaseController = 
'/*
 *  BaseController.es - Base class for all controllers
 */

public class BaseController extends Controller {

    public var title: String = "${NAME}"
    public var style: String

    function BaseController() {
        style = appUrl + "/web/style.css"
    }
}
'


    /*
     *****************  Controller template ***********************
     */
    public static const Controller = 
'
public class ${NAME}Controller extends BaseController {

    public var ${LOWER_MODEL}: ${MODEL}

    function ${NAME}Controller() {
    }

    use namespace action

    NEXT_ACTION
}
'


    /*
     *****************  ScaffoldController template ******************
     */
    public static const ScaffoldController = 
'
public class ${NAME}Controller extends BaseController {

    public var ${LOWER_MODEL}: ${MODEL}

    function ${NAME}Controller() {
    }

    use namespace action

    action function index() { 
        renderView("list")
    }

    action function list() { 
    }

    action function edit() {
        ${LOWER_MODEL} = ${MODEL}.find(params.id)
    }

    action function create() {
        ${LOWER_MODEL} = new ${MODEL}
        renderView("edit")
    }

    action function update() {
        if (params.commit == "Cancel") {
            redirect("list")

        } else if (params.commit == "Delete") {
            destroy()

        } else if (params.id) {
            ${LOWER_MODEL} = ${MODEL}.find(params.id)
            if (${LOWER_MODEL}.saveUpdate(params.${LOWER_MODEL})) {
                inform("${MODEL} updated successfully.")
                redirect("list")
            } else {
                /* Validation failed */
                renderView("edit")
            }

        } else {
            ${LOWER_MODEL} = new ${MODEL}(params.${LOWER_MODEL})
            if (${LOWER_MODEL}.save()) {
                inform("New ${LOWER_MODEL} created")
                redirect("list")
            } else {
                renderView("edit")
            }
        }
    }

    action function destroy() {
        ${MODEL}.remove(params.id)
        inform("${MODEL} " + params.id + " removed")
        redirect("list")
    }

    NEXT_ACTION
}
'


    /*
     *****************  ScaffoldListView template ******************
     */
    public static const ScaffoldListView = 
'<h1>${MODEL} List</h1>

<% table(${MODEL}.findAll(), {click: "edit"}) %>
<br/>
<% buttonLink("New ${MODEL}", "create") %>
'


    /*
     *****************  ScaffoldEditView template ******************
     */
    public static const ScaffoldEditView = 
'<h1><%= (${LOWER_MODEL}.id) ? "Edit" : "Create" %> ${MODEL}</h1>

<% form("update", ${LOWER_MODEL}) %>

    <table border="0">
    <% for each (name in ${MODEL}.columnNames) {
        if (name == "id") continue
        uname = name.toPascal()
    %>
        <tr><td>@@uname</td><td><% input(name) %></td></tr>
    <% } %>
    </table>

    <% button("OK", "commit") %>
    <% button("Cancel", "commit") %>
    <% if (${LOWER_MODEL}.id) button("Delete", "commit") %>
<% endform() %>
'


    /*
     *****************  ScaffoldView template ******************
     */
    public static const ScaffoldView = 
'<h1>View "${CONTROLLER}/${VIEW}" for Model ${MODEL}</h1>
<p>Edit in "views/${CONTROLLER}/${VIEW}.ejs"</p>
'


    /*
     ***********************  Action template ***********************
     */
    public static const Action = '
    action function ${NAME}() {
    }

'


    /*
     ***********************  Model template ***********************
     */
    public static const Model = 
'/*
 *  ${NAME}.es - ${NAME} Model Class
 */

public dynamic class ${NAME} implements Record {

    function ${NAME}(fields: Object = null) {
        constructor(fields)
    }
}
'


    /*
     ***********************  Migration template ***********************
     */
    public static const Migration = 
'/*
 *  ${COMMENT}
 */
require App
require ejs.db

public class Migration {

    function forward(db) {
${FORWARD}    }

    function backward(db) {
${BACKWARD}
    }
}
'



    /*
     **************************** README template ***********************
     */
    public static const Readme = 
'
README - Overview of files and documentation generated by ejsweb

These Directories are created via "ejsweb generate ${NAME}:"

    bin                       Programs and scripts
    config                    Configuration files
    controllers               Controller source
    db                        SQL databases and database scripts
    db/migrations             SQL database migration scripts
    doc                       Documentation for the application
    logs                      Log files
    messages                  Internationalization messages
    models                    Database models
    src                       Extra application source
    test                      Test files
    views                     View source files
    views/layouts             View layout files
    web                       Public web directory
    web/themes                Theme style sheet directory
    .ejs                      State files used by ejsweb
    .tmp                      Temporary files

These files are also created:

    config/compiler.ecf       Compiler options
    config/config.ecf         General application configuration 
    config/database.ecf       Database connector configuration 
    config/view.ecf           View connector configuration 
    views/layouts/default.ejs Default template page for all views
    web/layout.css            Default layout style sheet
    web/themes/default.css    Default theme style sheet
    web/images/banner.jpg     Default UI banner
'


    /*
     ***************************  View header and footer templates ******************
     */
    public static const ViewHeader = 
'

public dynamic class ${CONTROLLER}${VIEW}View extends View {
    function ${CONTROLLER}${VIEW}View(c: Controller) {
        super(c)
    }

    override public function render() {
'

    public static const ViewFooter = '
    }
}
'

    /*
     ***************************  src/App.es ****************************
     */
    public static const AppSrc = 
'
require ejs.db
require ejs.web

module App {
}
'

    /*
     ***************************  Default Layout templates ******************
     */
    public static const DefaultLayout = 
'<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
    <meta http-equiv="content-type" content="text/html;charset=UTF-8" />
    <title>@@title</title>
    <% stylesheet(["web/layout.css", "web/themes/default.css" ]); %>
    <% script(["web/js/jquery.js", "web/js/jquery.tablesorter.js", "web/js/jquery.ejs.js"]) %>
</head>

<body>
    <div class="top">
        <h1><a href="@@appUrl/">${NAME} Application</a></h1>
    </div>
    <div id="logo">EJScript&trade;</div>

    <% flash(["inform", "error", "message", "warning"]) %>
    <div class="content">
        <%@ content %>
    </div>

    <div class="bottom">
        <p class="footnote">Powered by Ejscript&trade;</p>
    </div>
</body>
</html>
'


/* End of class Templates */
}


/*
 *  Ejsweb Parser - Parse an ejs file and emit a Ejscript compiled version
 *
 *  This parser handles embedded Ejscript using <% %> directives. It supports:
 *
 *    <%                    Begin an ejs directive section containing statements
 *    <%=                   Begin an ejs directive section that contains an expression to evaluate and substitute
 *    %>                    End an ejs directive
 *    <%@ include "file" %> Include an ejs file
 *    <%@ layout "file" %>  Specify a layout page to use. Use layout "" to disable layout management.
 *
 *  Directives for use outside of <% %> 
 *    @@var                 To expand the value of "var". Var can also be simple expressions (without spaces).
 *
 *  TODO implement these directives
 *    -%>                   Omit newline after tag
 */

/*
 *  Parser tokens
 */
class Token {
    public static const Err         = -1        /* Any input error */
    public static const Eof         = 0         /* End of file */
    public static const EjsTag      = 1         /* <% text %> */
    public static const Var         = 2         /* @@var */
    public static const Literal     = 3         /* literal HTML */
    public static const Equals      = 4         /* <%= expression */
    public static const Control     = 6         /* <%@ control */

    public static var tokens = [ "Err", "Eof", "EjsTag", "Var", "Literal", "Equals", "Control" ]
}


class EjsParser {

    private const ContentMarker: String         = "__ejs:CONTENT:ejs__"
    private const ContentPattern: RegExp        = new RegExp(ContentMarker)
    private const LayoutsDir: String            = "views/layouts"

    private var appBaseDir: String
    private var script: String
    private var pos: Number                     = 0
    private var lineNumber: Number              = 0
    private var layoutPage: String

    /*
     *  Main parser. Parse the script and return the compiled (Ejscript) result
     */
    public function parse(file: String, appDir: String, layout: string): String {

        var token: ByteArray = new ByteArray
        var out: ByteArray = new ByteArray
        var tid: Number

        appBaseDir = appDir;
        layoutPage = layout
        script = Path(file).readString()

        while ((tid = getToken(token)) != Token.Eof) {

            // print("getToken => " + Token.tokens[tid + 1] + " TOKEN => \"" + token + "\"")

            switch (tid) {
            case Token.Literal:
                out.write("\nwrite(\"" + token + "\");\n")
                break

            case Token.Var:
                /*
                 *  Trick to get undefined variables to evaluate to "".
                 *  Catenate with "" to cause toString to run.
                 */
                out.write("\nwrite(\"\" + ", token, ");\n")
                break

            case Token.Equals:
                out.write("\nwrite(\"\" + (", token, "));\n")
                break

            case Token.EjsTag:
                /*
                 *  Just copy the Ejscript code straight through
                 */
                //  TODO BUG ByteArray.write(ByteArray) is not working. Requires toString()
                out.write(token.toString())
                break

            case Token.Control:
                let args: Array = token.toString().split(/\s/g)
                let cmd: String = args[0]

                switch (cmd) {
                case "include":
                    let path = args[1].trim("'").trim('"')
                    let incPath = dirname(file).join(path)
                    /*
                     *  Recurse and process the include script
                     */
                    let inc: EjsParser = new EjsParser
                    out.write(inc.parse(incPath, appBaseDir, undefined))
                    break

                case "layout":
                    let path = args[1]
                    if (path == "" || path == '""') {
                        layoutPage = undefined
                    } else {
                        path = args[1].trim("'").trim('"').trim('.ejs') + ".ejs"
                        layoutPage = (path[0] == '/') ? path : (LayoutsDir + "/" + path)
                        if (! exists(layoutPage)) {
                            abort("Can't find layout page " + layoutPage)
                        }
                    }
                    break

                case "content":
                    out.write(ContentMarker)
                    break

                default:
                    abort("Bad control directive: " + cmd)
                }
                break

            default:
            case Token.Err:
                //  TODO - should report line numbers
                abort("Bad input token: " + token)

            }
        }

        if (layoutPage != undefined && layoutPage != file) {
            let layoutText: String = new EjsParser().parse(layoutPage, appBaseDir, layoutPage)
            return layoutText.replace(ContentPattern, out.toString())
        }
        return out.toString()
    }

    /*
     *  Get the next input token. Read from script[pos]. Return the next token ID and update the token byte array
     */
    function getToken(token: ByteArray): Number {

        var tid = Token.Literal

        token.flush()

        var c
        while (pos < script.length) {
            c = script[pos++]

            switch (c) {

            case '<':
                if (script[pos] == '%' && (pos < 2 || script[pos - 2] != '\\')) {
                    if (token.available > 0) {
                        pos--
                        return Token.Literal
                    }
                    pos++
                    eatSpace()
                    if (script[pos] == '=') {
                        /*
                         *  <%=  directive
                         */
                        pos++
                        eatSpace()
                        while ((c = script[pos]) != undefined && 
                                (c != '%' || script[pos+1] != '>' || script[pos-1] == '\\')) {
                            token.write(c)
                            pos++
                        }
                        pos += 2
                        return Token.Equals

                    } else if (script[pos] == '@') {
                        /*
                         *  <%@  directive
                         */
                        pos++
                        eatSpace()
                        while ((c = script[pos]) != undefined && (c != '%' || script[pos+1] != '>')) {
                            token.write(c)
                            pos++
                        }
                        pos += 2
                        return Token.Control

                    } else {
                        while ((c = script[pos]) != undefined && 
                                (c != '%' || script[pos+1] != '>' || script[pos-1] == '\\')) {
                            token.write(c)
                            pos++
                        }
                        pos += 2
                        return Token.EjsTag
                    }
                }
                token.write(c)
                break

            case '@':
                if (script[pos] == '@' && (pos < 1 || script[pos-1] != '\\')) {
                    if (token.available > 0) {
                        pos--
                        return Token.Literal
                    }
                    pos++
                    c = script[pos++]
                    while (c.isAlpha || c.isDigit || c == '[' || c == ']' || c == '.' || c == '$' || 
                            c == '_' || c == "'") {
                        token.write(c)
                        c = script[pos++]
                    }
                    pos--
                    return Token.Var
                }
                token.write(c)
                break

            case "\r":
            case "\n":
                lineNumber++
                token.write(c)
                tid = Token.Literal
                break

            default:
                //  TODO - triple quotes would eliminate the need for this
                if (c == '\"' || c == '\\') {
                    token.write('\\')
                }
                token.write(c)
                break
            }
        }
        if (token.available == 0 && pos >= script.length) {
            return Token.Eof
        }
        return tid
    }

    function eatSpace(): Void {
        while (script[pos].isSpace) {
            pos++
        }
    }

    function abort(msg: String): Void {
        throw "ejsweb: " + msg + ". At line " + lineNumber
    }
}

/*
 *  Main program
 */
var eweb: EjsWeb = new EjsWeb
eweb.loadDefaults()

try {
    if (!eweb.parseArgs(App.args)) {
        eweb.usage()
    }
}
catch (e) {
    error("ejsweb: " + e)
    App.exit(2)
}

/*
 *  @copy   default
 *
 *  Copyright (c) Embedthis Software LLC, 2003-2009. All Rights Reserved.
 *  Copyright (c) Michael O'Brien, 1993-2009. All Rights Reserved.
 *
 *  This software is distributed under commercial and open source licenses.
 *  You may use the GPL open source license described below or you may acquire
 *  a commercial license from Embedthis Software. You agree to be fully bound
 *  by the terms of either license. Consult the LICENSE.TXT distributed with
 *  this software for full details.
 *
 *  This software is open source; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version. See the GNU General Public License for more
 *  details at: http://www.embedthis.com/downloads/gplLicense.html
 *
 *  This program is distributed WITHOUT ANY WARRANTY; without even the
 *  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *  This GPL license does NOT permit incorporating this software into
 *  proprietary programs. If you are unable to comply with the GPL, you must
 *  acquire a commercial license to use this software. Commercial licenses
 *  for this software and support services are available from Embedthis
 *  Software at http://www.embedthis.com
 *
 *  @end
 */

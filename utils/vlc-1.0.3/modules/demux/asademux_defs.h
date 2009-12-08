/* AUTOGENERATED FILE, DO NOT EDIT */
/* generated from "./imports" on 2007-08-31T13:23:59+00:00 */

void preparse_add()
{
#define insn_init { NULL, NULL, 0, { 0 } }
#define det(n,r) { static struct asa_import_detect d = { NULL }; \
		d.name = n; \
		if (!asa_pcre_compile(&d.re, r)) \
			asa_det_last = &(*asa_det_last = &d)->next; }
#define fmt_b(n,t) { static struct asa_import_format f = { NULL }; \
		struct asa_import_insn *i, **i0 = NULL; \
		f.name = n; f.target = t;
#define fmt_e() \
		}
#define insn(n,t) { static struct asa_import_insn ii = insn_init; \
		i = &ii; *n = i; n = &i->next; i->insn = t; }
#define insn_b(n, m, t, r) { struct asa_import_insn **m;\
		{ static struct asa_import_insn ii = insn_init; \
			i = &ii; ii.insn = t; \
			m = &ii.v.child.insns; \
		}\
		if (!asa_pcre_compile(&i->v.child.regex, r)) { \
			*n = i; n = &i->next;
#define insn_e() } }
#define insn_sg(n, t, r) { struct asa_repl **repl;\
		{ static struct asa_import_insn ii = insn_init; \
			i = &ii; ii.insn = t; \
			repl = &ii.v.sg.repl; \
		}\
		if (!asa_pcre_compile(&i->v.sg.regex, r)) { \
			*n = i; n = &i->next;
#define insn_sge() } }
#define repl(g, t) { static struct asa_repl r = { NULL, g, t }; \
		*repl = &r; repl = &r.next; }
#define insn_ts(n, t, d) { struct asa_tspec **tsp;\
		{ static struct asa_import_insn ii = insn_init; \
			i = &ii; ii.insn = t; ii.v.tspec.delta_select = d; \
			tsp = &ii.v.tspec.tsp; \
		}\
		*n = i; n = &i->next;
#define insn_tse() }
#define tsp(g, m, f) { static struct asa_tspec t = { NULL, g, m, f }; \
		*tsp = &t; tsp = &t.next; }

	det("qttext","^\\{QTtext\\}")
	det("rtf","^\\{\\\\rtf")
	det("viplay","^\\{\\* VIPLAY")
	det("zerog","^% Zero G")
	det("sst","^SST ")
	det("philips","^# PHILIPS SVCD DESIGNER")
	det("ulead","^#Ulead subtitle")
	det("sonicscenarist","^st_format\\s*\\d")
	det("dvdjunior","^Subtitle File Mark")
	det("captionsdat","^\\0\\r#")
	det("inscriber","^@@.*\\n@@\\d Created by URUSoft")
	det("ssa","(?mi)^(ScriptType:|\\[Script Info)")
	det("subrip","^\\d+\\s*\\n\\d\\d:\\d\\d:\\d\\d,\\d\\d\\d\\s+-->\\s+\\d\\d:\\d\\d:\\d\\d,\\d\\d\\d\\s*\\n")
	det("microdvd","^\\{\\d+\\}\\{\\d+\\}")
	det("sami","(?i)<SAMI")
	det("smil","(?i)<SMIL")
	det("smil_rt","(?i)<WINDOW")
	det("html","(?i)<HTML")
	det("jacosub","(?m)^#([DT]\\d+)")
	det("sasamis2k","(?m)^;(Env|Set)\\.")
	det("phoenix","^[ \\d]+,[ \\d]+, \".*\"")
	det("vkt","(?m)^\\{\\d+ .*\\}")
	det("e2","^\\[\\d+\\]\\[\\d+\\]")
	det("powerdivx","^\\{\\d+:\\d\\d:\\d\\d\\}\\{\\d+:\\d\\d:\\d\\d\\}")
	det("sbt","^\\d\\d:\\d\\d:\\d\\d\\s*\\n\\d\\d:\\d\\d:\\d\\d\\s*\\n\\s*\\n")
	det("karaokelrc","(?m)^\\[\\d\\d:\\d\\d\\.\\d\\d\\]")
	det("dks","^\\[\\d\\d:\\d\\d:\\d\\d\\]")
	det("aqtitle","^-->> \\d+\\s*\\n")
	det("panimator","^\\/(c|d \\d+ \\d+)\\s*\\n")
	det("tmplayer","^\\d\\d:\\d\\d:\\d\\d,\\d=")
	det("cap32","^\\d\\d:\\d\\d:\\d\\d:\\d\\d , \\d\\d:\\d\\d:\\d\\d:\\d\\d , ")
	det("not_encore","(?m)^\\d\\d:\\d\\d:\\d\\d:\\d\\d \\d\\d:\\d\\d:\\d\\d:\\d\\d ")
	det("encore_ntsc","(?m)^\\d+ \\d+;\\d+;\\d+;\\d+ \\d+;\\d+;\\d+;\\d+ ")
	det("encore_pal","(?m)^\\d+ \\d+:\\d+:\\d+:\\d+ \\d+:\\d+:\\d+:\\d+ ")
	det("turbotitler","^\\d+:\\d\\d:\\d\\d\\.\\d\\d,\\d+:\\d\\d:\\d\\d\\.\\d\\d,")
	det("macdvdpro","^\\d\\d:\\d\\d:\\d\\d:\\d\\d\\t\\d\\d:\\d\\d:\\d\\d:\\d\\d\\t")
	det("powerpixel","^\\d\\d:\\d\\d:\\d\\d:\\d\\d\\t\\d\\d:\\d\\d:\\d\\d:\\d\\d\\r?\\n")
	det("ovr","^\\d\\d:\\d\\d:\\d\\d:\\d\\d [^[:digit:][:space:][:punct:]]")
	det("fab","^\\d\\d:\\d\\d:\\d\\d:\\d\\d  \\d\\d:\\d\\d:\\d\\d:\\d\\d\\s*\\n[^[:digit:][:space:][:punct:]]")
	det("sonicdvd","^\\d{4}  \\d\\d:\\d\\d:\\d\\d:\\d\\d  \\d\\d:\\d\\d:\\d\\d:\\d\\d  ")
	det("koalaplayer","(?m)^\\d+:\\d\\d:\\d\\d:[^[:digit:][:space:][:punct:]]")
	det("subcreator1","^\\d+:\\d\\d:\\d\\d\\.\\d+:")
	fmt_b("subrip", 2)
		insn_b(i0, i1, ASAI_CHILD, "^\\d+\\s*\\n(\\d\\d):(\\d\\d):(\\d\\d),(\\d\\d\\d)\\s+-->\\s+(\\d\\d):(\\d\\d):(\\d\\d),(\\d\\d\\d)\\s*\\r?\\n")
		insn_ts(i1, 8, -1); 			tsp(1, 3600.000000, 0.000000)
			tsp(2, 60.000000, 0.000000)
			tsp(3, 1.000000, 0.000000)
			tsp(4, 0.001000, 0.000000)
		insn_tse()
		insn_ts(i1, 9, -1); 			tsp(5, 3600.000000, 0.000000)
			tsp(6, 60.000000, 0.000000)
			tsp(7, 1.000000, 0.000000)
			tsp(8, 0.001000, 0.000000)
		insn_tse()
		insn_b(i1, i2, ASAI_CHILD, "(?s)^(.*?)\\s*\\n\\s*\\n")
		insn(i2, ASAI_SELECT); i->v.select = 1;
		insn_sg(i2, 4, "\\\\")
			repl(-1, "\\\\")
		insn_sge()
		insn_sg(i2, 4, "\\{")
			repl(-1, "\\{")
		insn_sge()
		insn_sg(i2, 4, "\\}")
			repl(-1, "\\}")
		insn_sge()
		insn_sg(i2, 4, "\\n")
			repl(-1, "\\n")
		insn_sge()
		insn_sg(i2, 4, "<[Bb]>")
			repl(-1, "{\\b1}")
		insn_sge()
		insn_sg(i2, 4, "<\\/[Bb]>")
			repl(-1, "{\\b0}")
		insn_sge()
		insn_sg(i2, 4, "<[Ii]>")
			repl(-1, "{\\i1}")
		insn_sge()
		insn_sg(i2, 4, "<\\/[Ii]>")
			repl(-1, "{\\i0}")
		insn_sge()
		insn_sg(i2, 4, "<\\/(.*?)>")
			repl(-1, "{")
			repl(12, NULL)
			repl(-1, "3}")
		insn_sge()
		insn_sg(i2, 4, "&lt;")
			repl(-1, "<")
		insn_sge()
		insn_sg(i2, 4, "&gt;")
			repl(-1, ">")
		insn_sge()
		insn_sg(i2, 4, "&amp;")
			repl(-1, "&")
		insn_sge()
		insn_sg(i2, 4, "^")
			repl(-1, ",,,0,0,0,,")
		insn_sge()
		insn(i2, 6);
		insn(i2, 0);
		insn(i2, ASAI_BREAK); i->v.break_depth = 1;
		insn_e()
		insn_e()
	fmt_e()
	fmt_b("encore_pal", 1)
		insn(i0, ASAI_FPS); i->v.fps_value = 25.000000;
		insn_b(i0, i1, ASAI_CHILD, "^\\d+ (\\d+):(\\d+):(\\d+):(\\d+) (\\d+):(\\d+):(\\d+):(\\d+) (.*(\\n[^\\d].*)*)\\n")
		insn_ts(i1, 8, -1); 			tsp(1, 3600.000000, 0.000000)
			tsp(2, 60.000000, 0.000000)
			tsp(3, 1.000000, 0.000000)
			tsp(4, 0.000000, 1.000000)
		insn_tse()
		insn_ts(i1, 9, -1); 			tsp(5, 3600.000000, 0.000000)
			tsp(6, 60.000000, 0.000000)
			tsp(7, 1.000000, 0.000000)
			tsp(8, 0.000000, 1.000000)
		insn_tse()
		insn(i1, ASAI_SELECT); i->v.select = 9;
		insn_sg(i1, 4, "\\/\\/")
			repl(-1, "\x0a")
		insn_sge()
		insn(i1, 6);
		insn(i1, 0);
		insn_e()
	fmt_e()
	fmt_b("encore_ntsc", 1)
		insn(i0, ASAI_FPS); i->v.fps_value = 29.969999;
		insn_b(i0, i1, ASAI_CHILD, "^\\d+ (\\d+);(\\d+);(\\d+);(\\d+) (\\d+);(\\d+);(\\d+);(\\d+) (.*(\\n[^\\d].*)*)\\n")
		insn_ts(i1, 8, -1); 			tsp(1, 3600.000000, 0.000000)
			tsp(2, 60.000000, 0.000000)
			tsp(3, 1.000000, 0.000000)
			tsp(4, 0.000000, 1.000000)
		insn_tse()
		insn_ts(i1, 9, -1); 			tsp(5, 3600.000000, 0.000000)
			tsp(6, 60.000000, 0.000000)
			tsp(7, 1.000000, 0.000000)
			tsp(8, 0.000000, 1.000000)
		insn_tse()
		insn(i1, ASAI_SELECT); i->v.select = 9;
		insn_sg(i1, 4, "\\/\\/")
			repl(-1, "\x0a")
		insn_sge()
		insn(i1, 6);
		insn(i1, 0);
		insn_e()
	fmt_e()
	fmt_b("microdvd", 1)
		insn_b(i0, i1, ASAI_CHILD, "^\\{\\s*(\\d+)\\}\\{\\s*(\\d+)\\}(.*?)\\s*\\n")
		insn_ts(i1, 8, -1); 			tsp(1, 0.000000, 1.000000)
		insn_tse()
		insn_ts(i1, 9, -1); 			tsp(2, 0.000000, 1.000000)
		insn_tse()
		insn(i1, ASAI_SELECT); i->v.select = 3;
		insn_sg(i1, 4, "\\|")
			repl(-1, "\x0a")
		insn_sge()
		insn(i1, 6);
		insn(i1, 0);
		insn_e()
	fmt_e()
	fmt_b("vkt", 1)
		insn_b(i0, i1, ASAI_CHILD, "^#.*\\n")
		insn_e()
		insn_b(i0, i1, ASAI_CHILD, "^{(\\d+) (.*)}\\s*\\n")
		insn_ts(i1, 9, -1); 			tsp(1, 0.000000, 1.000000)
		insn_tse()
		insn(i1, 0);
		insn_ts(i1, 8, -1); 			tsp(1, 0.000000, 1.000000)
		insn_tse()
		insn(i1, ASAI_SELECT); i->v.select = 2;
		insn(i1, 6);
		insn_e()
	fmt_e()

}

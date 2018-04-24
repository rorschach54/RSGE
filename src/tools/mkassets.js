const fs = require("fs");
const path = require("path");

var inpath = process.argv[2];
var incpath = process.argv[3];
var incprefix = process.argv[4];
var outpath = process.argv[5];
var name = process.argv[6] || "rsge_assets";

if(incprefix[incprefix.length-1] != "/") incprefix += "/";

var files = JSON.parse(fs.readFileSync(inpath).toString());

/* Generate C source */
var out = "#include <"+incprefix+name+".h>\n\n";

out += "rsge_error_e "+name+"_find(rsge_assets_t* assets,rsge_asset_t* file,char* name) {\n";
for(var i = 0;i < files.length;i++) {
	var data = fs.readFileSync(files[i]["path"]);
	
	out += "\tif(!strcmp(name,\""+files[i]["name"]+"\")) {\n";
	out += "\t\tfile->name = \""+files[i]["name"]+"\";\n";
	out += "\t\tfile->size = "+data.length+";\n";
	out += "\t\tfile->data = (char["+data.length+"]){ ";
	for(var o = 0;o < data.length;o++) {
		out += "0x"+Buffer.from([data[o]]).toString("hex");
		if(o+1 < data.length) out += ",";
	}
	out += " };\n";
	out += "\t\treturn RSGE_ERROR_NONE;\n";
	out += "\t}\n";
}
out += "\treturn RSGE_ERROR_NOASSET;\n";
out += "}\n";

out += "\nrsge_assets_t "+name+" = { .find = "+name+"_find };";
fs.writeFileSync(outpath,out);

/* Generate header */
out = "#ifndef __"+name.toUpperCase().split(path.sep).join("_")+"_\n";
out += "#define __"+name.toUpperCase().split(path.sep).join("_")+"_ 1\n\n";

out += "#include <rsge/assets/file.h>\n";
out += "#include <rsge/assets/def.h>\n\n";

out += "extern rsge_assets_t "+name+";\n";

out += "\n#endif";
fs.writeFileSync(path.join(incpath,name+".h"),out);

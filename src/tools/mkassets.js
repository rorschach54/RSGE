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

for(var i = 0;i < files.length;i++) {
	out += "rsge_asset_file_t "+name+"_"+files[i]["name"].split("/").join("_").split(".").join("__")+" = {";
	out += "\n\t.name = \""+files[i]["name"]+"\",";
	var data = fs.readFileSync(files[i]["path"]);
	out += "\n\t.data = (unsigned char["+data.length+"]) {";
	for(var o = 0;o < data.length;o++) {
		out += "0x"+Buffer.from([data[o]]).toString("hex");
		if(o+1 < data.length) out += ",";
	}
	out += "},";
	out += "\n\t.size = "+data.length;
	out += "\n};\n";
}

out += "\nrsge_assets_t "+name+" = {\n\t.files = {";

for(var i = 0;i < files.length;i++) {
	out += "\n\t\t&"+name+"_"+files[i]["name"].split("/").join("_").split(".").join("__");
	if(i+1 < files.length) out += ",";
}

out += "\n\t},\n\t.count = "+files.length+"\n};";
fs.writeFileSync(outpath,out);

/* Generate header */
out = "#ifndef __"+name.toUpperCase().split(path.sep).join("_")+"__\n";
out += "#define __"+name.toUpperCase().split(path.sep).join("_")+"__ 1\n\n";

out += "#include <rsge/assets/file.h>\n";
out += "#include <rsge/assets/def.h>\n";

out += "extern rsge_assets_t "+name+";\n";

out += "\n#endif";
fs.writeFileSync(path.join(incpath,name+".h"),out);
const fs = require('fs')

fs.readFile('./data/code.S', 'utf8' , (err, data) => {
	if (err) {
		console.error(err)
		return;
	}
	parse(data)
});


var label = [];
var codes = [];

function labelAt(name) {
	return label.find((e) => {
		return e["name"] == name;
	})["address"];
}


function parse(data) {
	console.log(data);

	var lines = data.split("\n");

	var count = 0;

	lines.forEach((line) => {
		if(line.length > 0) {
			if(line.startsWith("\t")) {
				line = line.substring(1);

				var tokens = line.split(" ");
				var op = tokens.shift();

				codes.push({"op": op, "args": tokens});
				count++;
			} else {
				line = line.slice(0, -1);
				label.push({"name": line, "address": count});
			}
		}
	});
	execute(labelAt("main"), label, codes);
}



function execute(pc, label, codes) {

	var varlist = [];
	var stack = [];

	loop:
	while(true) {
		var code = codes[pc];

		if(code) {
			var op = code["op"];
			var args = code["args"];

			switch(op) {
				case "load_addr": {
					var addr = varlist.findIndex((e) => {
						return e["name"] == args[0];
					});

					stack.push(addr);
				}
				break;

				case "deref": {
					var addr = stack.pop();
					var variable = varlist[addr];

					stack.push(variable["value"]);
				}
				break;

				case "store": {
					var data = stack.pop();

					varlist.push({"name": args[0], "value": data});
				}
				break;

				case "load": {
					var found = varlist.find((e) => {
						return e["name"] == args[0];
					});
					stack.push(found["value"]);
				}
				break;

				case "push_int": {
					stack.push(parseInt(args[0]));
				}
				break;

				case "add": {
					var data1 = stack.pop();
					var data2 = stack.pop();

					stack.push(data1 + data2);
				}
				break;

				case "sub": {
					var data1 = stack.pop();
					var data2 = stack.pop();

					stack.push(data1 - data2);
				}
				break;

				case "mul": {
					var data1 = stack.pop();
					var data2 = stack.pop();

					stack.push(data1 * data2);
				}
				break;

				case "call": {
					var name = args[0];

					console.log(labelAt(name));
				}
				break;

				default: {
					console.log(`Unknown instruction ${op}`);
					break loop;
				}
				break;
			}

			pc++;

		} else {
			console.log(`Program ended`);
			break loop;
		}
	}

	console.log(stack, varlist);
}
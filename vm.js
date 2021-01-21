var fs = require("fs");

var ins = fs.readFileSync('./data/out.code', {
	encoding:'utf8', 
	flag:'r'
});

console.log(ins);

loadInstructions(ins);

function loadInstructions(ins) {
	var payload = [];

	var functions = ins.split("@");
	for(var f in functions) {
		var array_func;
		var ops = functions[f].split("\n");
		for(var o in ops) {
			var op = ops[o];
			if(op) {
				var tokens = op.split(" ");
				if(tokens[0] == "func") {
					array_func = {"function": tokens[1], "data": []};
				} else {
					tokens[0] = tokens[0].trim();
					array_func["data"].push(tokens);
				}
			}
		}
		if(array_func) {
			payload.push(array_func);
		}
	}

	function getFunction(name) {
		for(var i in payload) {
			var func = payload[i];
			if(func["function"] == name) {
				return func;
			}
		}
	}

	function exec(func, stack) {
		var ops = func["data"];
		var variables = [];
		function getVar(name) {
			for(var i in variables) {
				var varr = variables[i];
				if(varr["name"] == name) {
					return varr["data"];
				}
			}
		}
		function putVar(name, data) {
			for(var i in variables) {
				var varr = variables[i];
				if(varr["name"] == name) {
					variables[i]["data"] = data;
					return;
				}
			}
			variables.push({"name":name, "data": data});
		}
		var i = 0;
		while(i < ops.length) {
			var op = ops[i];
			switch(op[0]) {
				case "push": {
					stack.push(parseInt(op[1]));
				}
				break;
				case "push_str": {
					stack.push(op[1]);
				}
				break;
				case "str_concat": {
					var last = stack.pop();
					var second_last = stack.pop();
					var concat = second_last + last;
					stack.push(concat);
				}
				break;
				case "store": {
					var last = stack.pop();
					putVar(op[1], last);
				}
				break;
				case "load": {
					stack.push(getVar(op[1]));
				}
				break;
				case "add": {
					var last = stack.pop();
					var second_last = stack.pop();
					var add = last + second_last;
					stack.push(add);
				}
				break;
				case "sub": {
					var last = stack.pop();
					var second_last = stack.pop();
					var sub = last - second_last;
					stack.push(sub);
				}
				break;
				case "mul": {
					var last = stack.pop();
					var second_last = stack.pop();
					var mul = last * second_last;
					stack.push(mul);
				}
				break;
				case "div": {
					var last = stack.pop();
					var second_last = stack.pop();
					var div = last / second_last;
					stack.push(div);
				}
				break;
				case "call": {
					if(op[1] == "print") {
						var last = stack.pop();
						console.log(last);
					} else {
						var f = getFunction(op[1]);
						const stack_copy = [];
						Object.assign(stack_copy, stack);
						stack.pop();
						stack.pop();
						stack.push(exec(f, stack_copy));	
					}
				}
				break;
				case "ret": {
					return stack.pop();
				}
				break;
			}
			i++;
		}
		console.log("-----STACK-----");
		console.log(stack);
		console.log("-----VARS-----");
		console.log(variables);
	}

	var stack = [];
	exec(getFunction("main"), stack);

}
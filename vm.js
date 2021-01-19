var ins = `
func main
	push_str
	push_str
	str_concat
	store a
	load a
	call printf
`;

loadInstructions(ins);

function loadInstructions(ins) {
	var payload = [];

	var each = ins.split("\n");
	var currentProc = "";
	for(var i in each) {
		var a = [];
		var tokens = each[i].split(" ");
		if(tokens[0] == "func") {
			currentProc = tokens[0];
			payload.push({"name": tokens[0], "data": a});
		} else {
			a.push(tokens);
		}
	}

	console.log(payload);
}
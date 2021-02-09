function strjoin:
	0: store 0
	1: store 1
	2: load 1
	3: push_i 1
	4: call 2
	5: store 3
	6: load 0
	7: push_i 1
	8: call 2
	9: store 4
	10: push_i 1
	11: load 4
	12: add
	13: load 3
	14: add
	15: newarray 2
	16: store 5
	17: push_i 0
	18: store 6
	19: push_i 0
	20: store 7
	21: push_i 0
	22: store 8
	23: load 3
	24: load 6
	25: cmplt
	26: push_i 0
	27: jmpifeq 41
	28: load 6
	29: arr_load 1
	30: load 8
	31: arr_store 5
	32: push_i 1
	33: load 6
	34: add
	35: store 6
	36: push_i 1
	37: load 8
	38: add
	39: store 8
	40: goto 23
	41: load 4
	42: load 7
	43: cmplt
	44: push_i 0
	45: jmpifeq 59
	46: load 7
	47: arr_load 0
	48: load 8
	49: arr_store 5
	50: push_i 1
	51: load 7
	52: add
	53: store 7
	54: push_i 1
	55: load 8
	56: add
	57: store 8
	58: goto 41
	59: load 5
	60: ret
function strjoinc:
	0: store 0
	1: store 1
	2: load 1
	3: push_i 1
	4: call 2
	5: store 3
	6: push_i 1
	7: push_i 1
	8: add
	9: load 3
	10: add
	11: newarray 2
	12: store 5
	13: push_i 0
	14: store 6
	15: push_i 0
	16: store 7
	17: push_i 0
	18: store 8
	19: load 3
	20: load 6
	21: cmplt
	22: push_i 0
	23: jmpifeq 37
	24: load 6
	25: arr_load 1
	26: load 8
	27: arr_store 5
	28: push_i 1
	29: load 6
	30: add
	31: store 6
	32: push_i 1
	33: load 8
	34: add
	35: store 8
	36: goto 19
	37: load 0
	38: load 8
	39: arr_store 5
	40: load 5
	41: ret
function strlen:
	0: store 9
	1: push_i 0
	2: store 10
	3: load 9
	4: push_i 1
	5: call 11
	6: store 12
	7: load 12
	8: load 10
	9: cmplt
	10: push_i 0
	11: jmpifeq 26
	12: push_i 0
	13: load 10
	14: arr_load 9
	15: cmpeq
	16: push_i 0
	17: jmpifeq 21
	18: load 10
	19: ret
	20: goto 21
	21: push_i 1
	22: load 10
	23: add
	24: store 10
	25: goto 7
	26: load 10
	27: ret
function strstr:
	0: store 13
	1: store 14
	2: push_i 0
	3: store 10
	4: load 14
	5: push_i 1
	6: call 2
	7: load 10
	8: cmplt
	9: push_i 0
	10: jmpifeq 27
	11: load 10
	12: arr_load 14
	13: store 15
	14: load 13
	15: load 15
	16: cmpeq
	17: push_i 0
	18: jmpifeq 22
	19: push_i 1
	20: ret
	21: goto 22
	22: push_i 1
	23: load 10
	24: add
	25: store 10
	26: goto 4
	27: push_i 0
	28: ret
function strcmp:
	0: store 0
	1: store 1
	2: load 1
	3: push_i 1
	4: call 2
	5: store 16
	6: load 0
	7: push_i 1
	8: call 2
	9: store 17
	10: load 17
	11: load 16
	12: cmpeq
	13: push_i 0
	14: jmpifeq 40
	15: push_i 0
	16: store 10
	17: load 16
	18: load 10
	19: cmplt
	20: push_i 0
	21: jmpifeq 37
	22: load 10
	23: arr_load 0
	24: load 10
	25: arr_load 1
	26: cmpneq
	27: push_i 0
	28: jmpifeq 32
	29: push_i 1
	30: ret
	31: goto 32
	32: push_i 1
	33: load 10
	34: add
	35: store 10
	36: goto 17
	37: push_i 0
	38: ret
	39: goto 40
	40: push_i 1
	41: ret
function itos:
	0: store 18
	1: load 18
	2: push_i 1
	3: call 19
	4: ret
function sizeof:
	0: store 20
	1: load 20
	2: push_i 1
	3: call 21
	4: ret
function explode:
	0: store 22
	1: store 23
	2: store 24
	3: push_i 1000
	4: newarray 2
	5: store 25
	6: load 24
	7: push_i 1
	8: call 2
	9: store 26
	10: push_i 0
	11: store 10
	12: push_i 0
	13: store 27
	14: load 26
	15: load 10
	16: cmplt
	17: push_i 0
	18: jmpifeq 92
	19: push_i 0
	20: store 28
	21: load 25
	22: load 10
	23: arr_load 24
	24: push_i 2
	25: call 29
	26: store 30
	27: load 30
	28: store 25
	29: push_i 1
	30: load 10
	31: add
	32: store 10
	33: load 28
	34: arr_load 23
	35: load 10
	36: arr_load 24
	37: cmpeq
	38: push_i 0
	39: jmpifeq 91
	40: push_i 0
	41: load 10
	42: arr_load 24
	43: cmpeq
	44: push_i 0
	45: jmpifeq 49
	46: push_s 31
	47: ret
	48: goto 49
	49: push_i 0
	50: load 28
	51: arr_load 23
	52: cmpeq
	53: push_i 0
	54: jmpifeq 58
	55: push_s 31
	56: ret
	57: goto 58
	58: push_i 1
	59: load 10
	60: add
	61: store 10
	62: push_i 1
	63: load 28
	64: add
	65: store 28
	66: load 23
	67: push_i 1
	68: call 2
	69: load 28
	70: cmpeq
	71: push_i 0
	72: jmpifeq 90
	73: load 22
	74: load 27
	75: cmpeq
	76: push_i 0
	77: jmpifeq 81
	78: load 25
	79: ret
	80: goto 81
	81: push_s 31
	82: store 32
	83: load 32
	84: store 25
	85: push_i 1
	86: load 27
	87: add
	88: store 27
	89: goto 90
	90: goto 33
	91: goto 14
	92: push_s 31
	93: ret
function stoi:
	0: store 9
	1: push_i 0
	2: store 33
	3: push_i 1
	4: store 34
	5: push_i 0
	6: store 10
	7: push_i 45
	8: load 9
	9: push_i 0
	10: push_i 2
	11: call 35
	12: cmpeq
	13: push_i 0
	14: jmpifeq 24
	15: load 34
	16: push_i 2
	17: sub
	18: store 34
	19: push_i 1
	20: load 10
	21: add
	22: store 10
	23: goto 24
	24: load 9
	25: push_i 1
	26: call 2
	27: load 10
	28: cmplt
	29: push_i 0
	30: jmpifeq 47
	31: push_i 48
	32: load 9
	33: load 10
	34: push_i 2
	35: call 35
	36: sub
	37: push_i 10
	38: load 33
	39: mul
	40: add
	41: store 33
	42: push_i 1
	43: load 10
	44: add
	45: store 10
	46: goto 24
	47: load 33
	48: load 34
	49: mul
	50: ret
function rand:
	0: store 12
	1: store 36
	2: load 36
	3: load 12
	4: push_i 2
	5: call 37
	6: ret
function new_socket:
	0: push_i 0
	1: call 38
	2: ret
function socket_bind:
	0: store 39
	1: store 40
	2: store 41
	3: load 41
	4: load 40
	5: load 39
	6: push_i 3
	7: call 42
	8: ret
function socket_accept:
	0: store 41
	1: load 41
	2: push_i 1
	3: call 43
	4: ret
function socket_read:
	0: store 44
	1: store 41
	2: load 41
	3: load 44
	4: push_i 2
	5: call 45
	6: ret
function socket_write:
	0: store 46
	1: store 24
	2: store 41
	3: load 41
	4: load 24
	5: load 46
	6: push_i 3
	7: call 47
	8: ret
function socket_close:
	0: store 41
	1: load 41
	2: push_i 1
	3: call 48
function exec:
	0: store 24
	1: load 24
	2: push_i 1
	3: call 49
	4: ret
function fopen:
	0: store 50
	1: store 51
	2: load 51
	3: load 50
	4: push_i 2
	5: call 52
	6: ret
function fread:
	0: store 44
	1: store 53
	2: load 53
	3: load 44
	4: push_i 2
	5: call 54
	6: ret
function fclose:
	0: store 41
	1: load 41
	2: push_i 1
	3: call 55
function printf:
	0: store 9
	1: load 9
	2: push_i 1
	3: call 56
function printc:
	0: store 5
	1: load 5
	2: push_i 1
	3: call 57
function main:
	0: push_s 58
	1: push_i 1
	2: call 59
	3: push_i 8012
	4: store 39
	5: push_i 0
	6: call 60
	7: store 41
	8: push_i 1
	9: load 41
	10: push_s 61
	11: load 39
	12: push_i 3
	13: call 62
	14: cmpeq
	15: push_i 0
	16: jmpifeq 84
	17: push_s 63
	18: push_i 1
	19: call 59
	20: push_i 1
	21: push_i 0
	22: jmpifeq 83
	23: load 41
	24: push_i 1
	25: call 64
	26: store 65
	27: load 65
	28: push_i 8192
	29: push_i 2
	30: call 66
	31: store 67
	32: push_s 68
	33: push_s 69
	34: push_i 2
	35: call 70
	36: store 71
	37: push_i 1
	38: load 71
	39: cmpeq
	40: push_i 0
	41: jmpifeq 82
	42: load 71
	43: push_i 999999
	44: push_i 2
	45: call 72
	46: store 24
	47: load 24
	48: push_i 1
	49: call 11
	50: store 73
	51: push_s 74
	52: load 73
	53: push_i 1
	54: call 75
	55: push_i 2
	56: call 76
	57: push_s 77
	58: push_i 2
	59: call 76
	60: store 78
	61: load 65
	62: load 78
	63: load 78
	64: push_i 1
	65: call 2
	66: push_i 3
	67: call 79
	68: store 80
	69: load 65
	70: load 24
	71: load 73
	72: push_i 3
	73: call 79
	74: store 27
	75: load 65
	76: push_i 1
	77: call 81
	78: load 71
	79: push_i 1
	80: call 82
	81: goto 82
	82: goto 20
	83: goto 93
	84: push_s 83
	85: load 39
	86: push_i 1
	87: call 75
	88: strconcat
	89: push_s 84
	90: strconcat
	91: push_i 1
	92: call 59

#include "parse.h"

DataType get_common_type(Node *left, Node *right) {
	if(left->data_type == DATA_FLOAT || right->data_type == DATA_FLOAT) {
		return DATA_FLOAT;
	}
	if(left->data_type == DATA_INT || right->data_type == DATA_INT) {
		return DATA_INT;
	}

	return left->data_type;
}

void promote_type(Node *node) {
	if(!node || node->data_type) {
		return;
	}

	promote_type(node->left);
	promote_type(node->right);

	switch(node->type) {
		case AST_ADD:
		case AST_SUB:
		case AST_MUL:
		case AST_DIV:
		case AST_MOD: {
			DataType common = get_common_type(node->left, node->right);
			node->left  = new_cast(node->left, common);
			node->right = new_cast(node->right, common);

			node->data_type = node->left->data_type;
		}
		break;
		
		default: {
			node->data_type = DATA_INT;
		}
		break;
	}
}
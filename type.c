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

void normalize_type(Node *node) {
	if(!node || node->data_type) {
		return;
	}

	normalize_type(node->left);
	normalize_type(node->right);

	if(node->left && node->right) {

		DataType common = get_common_type(node->left, node->right);
		printf("%i\n", common);
		node->left->data_type  = common;
		node->right->data_type = common;

		node->data_type = node->left->data_type;
	}

}
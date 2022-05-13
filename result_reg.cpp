#include "AST.h"

/*
 * methods for working with "result_reg" attribute
 */


static int next_unique_number = 1;

int A_exp_::init_result_reg()  // generate unique numbers, starting from 1, each time this is called
{
	// for those who've taken CS355/356, this should be an atomic transaction, in a concurrent environment
	int my_number = next_unique_number;
	next_unique_number = next_unique_number + 1;
	// end of atomic transaction
	return my_number;
}

int A_exp_::init_unique_label(){
    int my_number = next_unique_number;
    next_unique_number = next_unique_number + 1;
    // end of atomic transaction
    return my_number;
}

int A_intExp_::sethi_ulman(){
	return 3;
}

int A_intExp_::init_result_reg() {
	return this->sethi_ulman();
}

int A_stringExp_::sethi_ulman(){
	return 3;
}

int A_stringExp_::init_result_reg(){
	return this->sethi_ulman();
}



int A_opExp_::sethi_ulman(){
	if (_left->result_reg() == _right->result_reg()){
		return _left->result_reg() + 1;
	} else {
		return std::max(_left->result_reg(), _right->result_reg());
	}
}

int A_opExp_::init_result_reg() {
    return this->sethi_ulman();
}

int A_callExp_::sethi_ulman(){
    return _args->init_result_reg();
}

int A_callExp_::init_result_reg(){
    return this->sethi_ulman();
}

int A_seqExp_::sethi_ulman(){
    return _seq->init_result_reg();
}
int A_seqExp_::init_result_reg(){
    return this->sethi_ulman();
}

int A_expList_::sethi_ulman(){

    if (_tail == 0){
        return _head->init_result_reg() + 1;
    } else{
        return std::max(_head->init_result_reg(), _tail->init_result_reg());
    }
}

int A_expList_::init_result_reg(){
    return this->sethi_ulman();
}

string A_expList_::result_reg_s() {
    if (this->_tail == 0) {
        return this->_head->result_reg_s();
    }
    else{
        return this->_tail->result_reg_s();
    }
}


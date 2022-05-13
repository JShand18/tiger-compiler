#include "AST.h"

/*
 * HERA_code methods
 */

const string indent_math = "    ";  // might want to use something different for, e.g., branches


string AST_node_::HERA_code()  // Default used during development; could be removed in final version
{
	string message = "HERA_code() requested for AST node type not yet having a HERA_code() method";
	EM_error(message);
	return "#error " + message;  //if somehow we try to HERA-C-Run this, it will fail
}

string AST_node_::HERA_data()  // Default used during development; could be removed in final version
{
	string message = "HERA_data() requested for AST node type not yet having a HERA_data() method";
	EM_error(message);
	return "#error " + message;  //if somehow we try to HERA-C-Run this, it will fail
}


string A_root_::HERA_code()
{
	return main_expr->HERA_data() + indent_math + "CBON()\n\n" + main_expr->HERA_code();  // was SETCB for HERA 2.3
}

string A_root_::HERA_data()
{
	return "";
}

string A_intExp_::HERA_code()
{
	return indent_math + "SET(" + this->result_reg_s() + ", " + str(value) +")\n";
}

string A_intExp_::HERA_data()
{
	return "";
}

static string HERA_math_op(Position p, A_oper op) // needed for opExp
{
	switch (op) {
	case A_plusOp:
		return "ADD";
	case A_minusOp:
	    return "SUB";
	case A_timesOp:
		return "MUL";	// was MULT for HERA 2.3
	default:
		EM_error("Unhandled case in HERA_math_op", false, p);
		return "Oops_unhandled_hera_math_op";
	}
}
string A_opExp_::HERA_code()
{
    // if the sethi-ulman numbers are the same move on of the operands into the result register of the operation
	if (_left->result_reg() >= _right->result_reg()){
        string my_code = indent_math + (HERA_math_op(pos(), _oper) + "(" +
                                        this->result_reg_s() + ", " +
                                        this->result_reg_s() + ", " +
                                        this->_right->result_reg_s()+ ")\n");
        string rescue_code = indent_math + "MOVE(" + this->result_reg_s() + ", " +
                           _left->result_reg_s() + ")\n";
        return _left->HERA_code() + rescue_code + _right->HERA_code() + my_code;
	} else {
        string my_code = indent_math + (HERA_math_op(pos(), _oper) + "(" +
                                        this->result_reg_s() + ", " +
                                        this->_left->result_reg_s() + ", " +
                                        this->result_reg_s()+ ")\n");
        string rescue_code = indent_math + "MOVE(" + this->result_reg_s() + ", " +
                           _right->result_reg_s() + ")\n";
        return _right->HERA_code() + rescue_code + _left->HERA_code() + my_code;
	}
}

string A_opExp_::HERA_data() {
    return _left->HERA_data() + _right->HERA_data();
}

string A_callExp_::HERA_code()
{
	string exp_code = _args->HERA_code();
//	if (_args->_tail != 0){
//	    if(_args->_head->result_reg() >= _args->_tail->_head->result_reg()){
//	        string rescue_code = indent_math + "MOVE(" + this->result_reg_s() + ", "
//	                            + _args->_head->result_reg_s() + ")\n";
//	        exp_code = exp_code + rescue_code;
//	    } else{
//
//	    }
//		exp_code = exp_code + _args->_tail->HERA_code();
//	}

    string move_instruction = indent_math + ("MOVE(R1, " +
                                             _args->_head->result_reg_s() + ")\n");

	if (str(_func) == "div" || str(_func) == "mod"){
	    move_instruction = move_instruction + indent_math + ("MOVE(R2, " +
                            this->result_reg_s() + ")\n");
	}

	string call_code = indent_math + ("CALL(FP_alt, " + str(_func) + ")\n") +
	                    indent_math + ("MOVE(" + this->result_reg_s() + ", R1)\n");

	return  exp_code + move_instruction + call_code;
}

string A_callExp_::HERA_data()
{
	return _args->_head->HERA_data();
}

string A_stringExp_::HERA_code()
{
    string string_code = indent_math + ("SET("+ this->result_reg_s() +
										", the_string"+ this->unique_label_s() +")\n");
	return string_code;
}

string A_stringExp_::HERA_data()
{
	string data_code = indent_math + "DLABEL(the_string"+ this->unique_label_s() +")\n" +
						indent_math + "LP_STRING(" + repr(value) + ")\n";

	return data_code;
}

string A_expList_::HERA_code()
{
	if (_tail == 0){
		return this->_head->HERA_code();
	} else{
            return this->_head->HERA_code() + indent_math + "MOVE(R" +
                    repr(this->init_result_reg()) + ", "+
                    this->_head->result_reg_s() + ")" +
                    "\n" + this->_tail->HERA_code();

	}
}

string A_expList_::HERA_data()
{
	if (_tail == 0){
		return this->_head->HERA_data();
	} else{
		return this->_head->HERA_data() + this->_tail->HERA_data(); 
	}
}

string A_seqExp_::HERA_code()
{
	return this->_seq->HERA_code() + indent_math + "MOVE("+this->result_reg_s()
									+", " + _seq->result_reg_s() + ")\n";
}

string A_seqExp_::HERA_data()
{
	return this->_seq->HERA_data();
}
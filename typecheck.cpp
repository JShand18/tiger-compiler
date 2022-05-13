#include "util.h"
#include "AST.h"
#include "errormsg.h"
#include "typecheck.h"

//
// Change this comment to describe the attributes you'll be using
//  to check types in tiger programs.
//

#define SYM_INFO_FOR_STDLIB

void typecheck(AST_node_ *root)
{
	// This function should take care of doing type checking,
	//  assuming "root" is the root of a tiger AST.
	// It should call on some functions to define the attributes
	//  related to type checking (and, in so doing, produce any necessary error messages).
	
}

Ty_ty A_root_::typecheck() {
    return main_expr->typecheck();
}

Ty_ty A_exp_::typecheck() {
    EM_error("Call typecheck on a node having method");
    return Ty_Error();
}


Ty_ty A_stringExp_::typecheck(){
    return Ty_String();
}

Ty_ty A_intExp_::typecheck(){
    return Ty_Int();

}

Ty_ty A_opExp_::typecheck(){
	switch (this->_oper)
	{
	case A_plusOp:
		if(_left->typecheck() != _right->typecheck()){
            EM_error("Addtion needs operands of the same type");
            return Ty_Error();
		} else{
		    return _left->typecheck();
		}
		break;
	
	case A_minusOp:
	    if(_left->typecheck() != Ty_Int() || _right->typecheck() != Ty_Int()){
	        EM_error("Subtraction needs integer operands");
            return Ty_Error();
	    }  else{
	        return Ty_Int();
	    }

	case A_timesOp:
        if(_left->typecheck() != Ty_Int() || _right->typecheck() != Ty_Int()){
            EM_error("Multiplication needs integer operands");
            return Ty_Error();
        }  else{
            return Ty_Int();
        }

	default:
	    EM_error("Unhandled case in HERA_math_op", false);
	    return Ty_Error();
	}

}

Ty_ty A_seqExp_::typecheck() {
    if (_seq == NULL){
        return Ty_Void();
    }
    else if (_seq->_tail == 0) {
        return this->_seq->_head->typecheck();
    }
    else{
        return this->_seq->_tail->typecheck();
    }
}

Ty_ty A_expList_::typecheck() {
    if (this->_tail == 0) {
        return this->_head->typecheck();
    }
    else{
        return this->_tail->typecheck();
    }
}

Ty_ty A_callExp_::typecheck(){
    return Ty_Void();
}
// The bodies of other type checking functions,
//  including any virtual functions you introduce into
//  the AST classes, should go here.


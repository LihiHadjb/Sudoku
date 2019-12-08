/*
 * GRB_Solver.c
 *
 *
 */


#include "GRB_Solver.h"



/*initialize the given string to clean "garbage" values in the memory allocated for it*/
void init_str(char *str){
	int i;
	for(i=0; i<(int)strlen(str); i++){
		str[i] = '\0';
	}
}


/*Returns a string that is used as a name for the constraint, according to its type what cell or value
 * this constraint represents*/
char* constr_name(int type, int index1, int index2, int board_size){
	char *name, *temp1, *temp2;
	name = (char*)malloc(sizeof(char)*(5+board_size));/*free somewhere!!!!!!!*/
	if(name == NULL) {memory_error("malloc");}
	temp1 = (char*) malloc(sizeof(char)*(1+board_size));
	if(temp1 == NULL) {memory_error("malloc");}
	temp2 =  (char*)malloc(sizeof(char)*(1+board_size));
	if(temp2 == NULL) {memory_error("malloc");}

	init_str(name);
	init_str(temp1);
	init_str(temp2);
	sprintf(temp1,"%d", index1);
	sprintf(temp2,"%d", index2);
	switch(type){

	case(CELL):

		name = strcat(name, "cell_(");
		name = strcat(name, temp1);
		name = strcat(name,",");
		name = strcat(name, temp2);
		name = strcat(name,")");
		free(temp1);
		free(temp2);
		return name;

	case(RANGE_B):

		name = strcat(name, "range_B");
		name = strcat(name, temp1);
		free(temp1);
		free(temp2);
		return name;

	case(RANGE_T):

		name = strcat(name, "range_T");
		name = strcat(name, temp1);
		free(temp1);
		free(temp2);
		return name;

	case(ROW):
		name = strcat(name, "row");
		break;
	case(COL):
		name = strcat(name, "col");
			break;
	case(BLOCK):
		name = strcat(name, "bl");
	}
	name = strcat(name, temp1);
	name = strcat(name, "_");
	name = strcat(name, temp2);
	free(temp1);
	free(temp2);
	return name;
}







/*Add the given Gurobi model a constraint that is composed of the indices specified in ind and
 * the coefficients specified in val. The size and name of the constraint are the matching
 * arguments of the function. The type and right-hand-side value of the enquality are
 *  given by the arguments type and rhs.
 *  The function returns 1 iff the operation succeeded*/
int add_constr_to_model(GRBenv *env, GRBmodel *model, int *ind, double *val, int size, char* name, char type, int rhs){
	int error;
	error = GRBaddconstr(model, size, ind, val, type, rhs, name);
		if (error) {
			printf("ERROR %d at constraint %s: %s\n", error,name, GRBgeterrormsg(env));
			return 0;
		}
	return 1;
}


/*Adds constraints that ensure the solution to every variable x in the model
 * satisfies 0<=x<=1.
 * This function is only used while solving with LP.
 * Return 1 iff the operation succeeded.
 *
 */
int add_range_constrs(GRBenv *env, GRBmodel *model, Options_Board *cells_lists, int board_size){
	int i, error;
	char *name;
	int *ind_arr;
	double *val_arr;
	val_arr = (double*)malloc(sizeof(double));
	if(val_arr == NULL) {memory_error("malloc");}
	ind_arr = (int*)malloc(sizeof(int));
	if(ind_arr == NULL) {memory_error("malloc");}
	val_arr[0] = 1;
	for(i=0; i<cells_lists->num_elems; i++){
		ind_arr[0] = i;

		/*add constraint x<=1*/
		error=0;
		name = constr_name(RANGE_B, i,i, board_size);
		error = add_constr_to_model(env, model, ind_arr, val_arr, 1, name, GRB_GREATER_EQUAL, 0);
		if(error){
			return 0;
		}

		/*add constraint x>=0*/
		error=0;
		name = constr_name(RANGE_T, i,i, board_size);
		error = add_constr_to_model(env, model, ind_arr, val_arr, 1, name, GRB_LESS_EQUAL, 1);
		if(error){
			return 0;
		}
	}
	free(ind_arr);
	free(val_arr);
	return 1;
}



/*Adds the index of the currently first variable (element) in the list of the legal values of
 * cell (col, row) if its value matched val, in order to "collect" all the relevant variables for the constraint and add its
 * to the val_arr, ind_arr arrays.
 * We first pop out the first element out of the queue and then push it back to the end, so that
 * after traversing all the values of val (in an ascending order), the list is back to its original state.
 * Since variables are inserted to each queue in an ascending order and poped only when there is a match to val,
 *  it is sufficient to check if the first element has the value we are looking for in order to determine if it should be add to the constraint of the
 * value specified in val.
 * The argument i indicates the index in val_arr, ind_arr to which the variable should be added*/
void add_var(Options_Board *cells_lists, int col, int row, int val,double *val_arr, int *ind_arr, int *i){
	Option* elem;
	if(!is_empty_Q(CO_get_cell(cells_lists, col, row)) && peek_value(cells_lists, col, row) == val){
		ind_arr[*i] = peek_index(cells_lists, col, row);
		val_arr[*i] = 1;
		elem = remove_first_from_board(cells_lists, col, row);
		add_elem_to_board(cells_lists, col, row, elem);
		*i = *i+1;
	}
}

/*add valiables to the constraint built in val_arr and ind_arr for a colomn constraint of colomn
 * col with value val, according to cells_lists*/
void vars_by_col(Options_Board *cells_lists, int col, int val, int board_size, double *val_arr, int *ind_arr){
	int j, i=0;
	for(j=1; j<=board_size; j++){
		add_var(cells_lists, col, j, val, val_arr, ind_arr, &i);
	}
}


/*add valiables to the constraint built in val_arr and ind_arr for a row constraint of
 * row with value val, according to cells_lists*/
void vars_by_row(Options_Board *cells_lists, int row, int val, int board_size, double *val_arr, int *ind_arr){
	int j, i=0;
	for(j=1; j<=board_size; j++){
		add_var(cells_lists, j, row, val, val_arr, ind_arr, &i);
	}
}

/*add valiables to the constraint built in val_arr and ind_arr for a block constraint of block
 * with the index in index (blocks are counted left to right and top to bottom)
 * with value val, according to cells_lists*/
void vars_by_block(Options_Board *cells_lists, int index, int val, int m, int n,double *val_arr, int *ind_arr){
	int j,k, i=0;
	int first_col, first_row;
	first_col_row_in_block(m, n,index, &first_col, &first_row);
	for(j=0; j<m; j++){
		for(k=0; k<n; k++){
			add_var(cells_lists, first_col+k, first_row+j, val, val_arr, ind_arr, &i);
		}
	}
}

/*create a constraint that is composed of the legal values for cell col,row and add it to the model,
 * according to cells_lists*/
int add_cell_constr(GRBenv *env, GRBmodel *model, Options_Board *cells_lists,  int col, int row, int board_size){
	int i=0, val, j, size, error;
	char *name;
	double *val_arr;
	int *ind_arr;
	size = CO_get_list_size(cells_lists, col, row);
	val_arr = malloc(sizeof(double)*size);
	if(val_arr == NULL) {memory_error("malloc");}
	ind_arr = malloc(sizeof(int)*size);
	if(ind_arr == NULL) {memory_error("malloc");}
	name = constr_name(CELL, col, row, board_size);
	for(j=1; j<=size; j++){
		val = peek_value(cells_lists, col, row);
		add_var(cells_lists,  col,  row,  val,val_arr, ind_arr, &i);
	}

	error = add_constr_to_model(env, model, ind_arr, val_arr, size, name, GRB_EQUAL,1);
	if(error){
		return 0;
	}

	free(val_arr);
	free(ind_arr);
	free(name);
	return 1;
}


/*add to model constraints that ensure every cell gets exactly one value in the ILP case or
 * that its possibles values scores sum to 1 in the LP case*/
void constrs_by_cell(GRBenv *env, GRBmodel *model, Options_Board *cells_lists, Board *board){
	int i, j;
	int board_size = board->m * board->n;
	for(i=1; i<=board_size; i++){
		for(j=1; j<=board_size; j++){
			if(is_empty_cell(board, j, i)){
				add_cell_constr(env, model, cells_lists, j,  i,  board_size);
			}
		}
	}

}


/*add to model constraints that ensure every empty cell in every row/col/block (according to type)
  *  gets exactly one value in the ILP case or that its possibles values scores sum to 1 in the LP case*/


int add_constrs_by_type(Board *board, GRBenv *env, GRBmodel *model, Options_Board *cells_lists, int type, int m, int n){
	int index, add;
	int board_size = m*n;
	int val, size;
	char *name;
	int *ind_arr;
	double *val_arr;
	for(index=1; index<=board_size; index++){
		for(val=1; val<=board_size; val++){
			name = constr_name(type, index, val, board_size);
			/*allocate arrays for the current constraint*/
			size = cnt_num_occurs(cells_lists, val, index, type, m, n);
			val_arr = malloc(sizeof(double)*size);
			if(val_arr == NULL) {memory_error("malloc");}
			ind_arr = malloc(sizeof(int)*size);
			if(ind_arr == NULL) {memory_error("malloc");}
			add = 0;

			switch(type){
			case(ROW):
				if(!in_row(board, index, val)){
					add = 1;
					vars_by_row(cells_lists, index, val, board_size, val_arr, ind_arr);
				}
				break;
			case(COL):
				if(!in_col(board,index, val)){
					add = 1;
					vars_by_col(cells_lists, index, val, board_size, val_arr, ind_arr);
				}
				break;
			case(BLOCK):
				if(!in_block(board, index, val)){
					add = 1;
					vars_by_block(cells_lists, index, val, m, n, val_arr, ind_arr);
				}
			}
			if(add){
				add_constr_to_model(env, model, ind_arr, val_arr, size, name, GRB_EQUAL,1);
			}

			/* lihi if(size != 0){
				print_constr(val_arr, ind_arr, name, size);
				add_constraint_to_model(env, model, ind_arr, val_arr, size, name);
			}*/
			free(val_arr);
			free(ind_arr);
			free(name);

		}
	}
	return 1;
}






/*add constraints to the model according to the current board state*/
int add_constraints(GRBenv *env, GRBmodel *model, Options_Board *opts_board, int m, int n, int is_ILP, Board *board){
	int board_size = m*n;
	constrs_by_cell(env, model,opts_board,/* val,ind,*/ board);
	add_constrs_by_type(board, env, model, opts_board, ROW, m, n/* ind, val*/);
	add_constrs_by_type(board,env, model, opts_board, COL, m, n/* ind, val*/);
	add_constrs_by_type(board,env, model, opts_board, BLOCK, m, n/* ind, val*/);
	if(is_ILP == LP){
		add_range_constrs(env, model, opts_board, /*val, ind,*/ board_size);
	}

	return 1;
}



/*Set the objective function of the Gurobi model and environment given in env and model to be a function
 * that attempts to find an optimal solution that forms a solution to the board,
 * and according to the legal values of each cell provided in opts_board.
 * The function returns 1 iff the operation succeeded.
 *
 * When running ILP, we use a 0 objective function, as any solution to the system of
 * equalities can be translated into assignments that solve the board (since variables are binary).
 * When running LP, we maximize a sum of all the variables of the model, each with a random
 * coefficient in the range 1 to board_size, as this "encourages" the solution to give larger values (scores)
 * to some of the variables, which can be interpreted as choosing only one values for each cell.
 */
int set_obj_function(GRBenv *env, GRBmodel *model, double *obj, Options_Board *opts_board, int is_ILP){
	int i, error, coeff;
	int num_vars = opts_board->num_elems;
	int board_size = opts_board->m * opts_board->n;
	for(i=0; i<num_vars; i++){
		if(is_ILP == ILP){
			obj[i] = 0;
		}

		else{
			coeff = rand() % board_size + 1;
			obj[i] = coeff;
		}
	  }

	  /* Change objective sense to maximization */
	  error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
	  if (error) {
		  printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		  return -1;
	  }
	  return 1;
}

/*Compose the vtype array needed for the Gurobi model to define the type of each variable.
 * The function add num_vars variables to vtype, according to the solving method: for
 * ILP we use binary variables and for LP we use continuous variables*/
 void add_vars(char *vtype, int num_vars, int is_ILP){
	  int i;
	  for (i=0; i<num_vars; i++){
		  if(is_ILP == ILP){
			  vtype[i] = GRB_BINARY;
		  }
		  else{/*LP*/
			  vtype[i] = GRB_CONTINUOUS;
		  }
	  }
}



/*assumes the board is not erroneous and that there are no obvious cells!*/
int call_GRB(Board *board, double **sol, int is_ILP, Options_Board *opts_board)
{
  GRBenv   *env   = NULL;
  GRBmodel *model = NULL;
  int num_vars;
  int       error = 0;
  double    *obj;
  char      *vtype;
  int       optimstatus;
  double    objval;
  int result = 0;
  int m = board->m;
  int n = board->n;
  int board_size = m*n;


  init_Options_Board(board, opts_board, board_size);
  num_vars = opts_board->num_elems;

  /*allocate memory for the arrays*/

  obj = (double*)malloc(sizeof(double) * (num_vars));
  if(obj == NULL) {memory_error("malloc");}
  vtype = (char*)malloc(sizeof(char) * (num_vars));
  if(vtype == NULL) {memory_error("malloc");}
  init_sol(num_vars, sol);


  /* Create environment - log file is mip1.log */
  error = GRBloadenv(&env, "mip1.log");
  if (error) {
	  grb_error_mssg();
	  printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
	  return 0;
  }

  error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
  if (error) {
	  grb_error_mssg();
	  printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
	  return 0;
  }

  /* Create an empty model named "mip1" */
  error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
  if (error) {
	  grb_error_mssg();
	  printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
	  return 0;
  }


  set_obj_function(env, model, obj, opts_board, is_ILP);

  add_vars(vtype, num_vars, is_ILP);


  /* add variables to model */
  	  error = GRBaddvars(model, num_vars, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
  	  if (error) {
  		grb_error_mssg();
  		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
  		return 0;
  	  }
  /* update the model - to integrate new variables */
  	  error = GRBupdatemodel(model);
  	  if (error) {
  		grb_error_mssg();
  		printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
  		return 0;
  	  }


  add_constraints(env, model, opts_board,/* ind, val,*/ m, n, is_ILP, board);



  /* Optimize model */
  error = GRBoptimize(model);
  if (error) {
	  grb_error_mssg();
	  printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
	  return 0;
  }

  /* Write model to 'mip1.lp' - this is not necessary but very helpful */
  error = GRBwrite(model, "mip1.lp");
  if (error) {
	  grb_error_mssg();
	  printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
	  return 0;
  }

  /* Get solution information */
  error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) {
  	  grb_error_mssg();
  	  printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
  	  return 0;
    }


  /* check if a solution was found */
  if (optimstatus == GRB_OPTIMAL) {
	  result = 1;

	    /* get the objective -- the optimal result of the function */
	    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
	    if (error) {
	  	  grb_error_mssg();
	  	  printf("ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));
	  	  return 0;
	    }

	    /* get the solution - the assignment to each variable */
	    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, num_vars, *sol);
	    if (error) {
	  	  grb_error_mssg();
	  	  printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
	  	  return 0;
	    }

  }

  /* IMPORTANT !!! - Free model and environment */
  free(obj);
  free(vtype);
  GRBfreemodel(model);
  GRBfreeenv(env);

  return result;
}

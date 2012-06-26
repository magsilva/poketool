
declaration = directives 'EOF', 
              decl_specifiers 23 init_opt 31 [ declaration ] aux . 

	  init_opt = init_decl_list , * .

	  aux = ';' 'EOF', 'EOF'.

directives = '#' 'include' aux1_directives .
      aux1_directives = 'STRING' ,
			'BINOP' 'IDENT' '.' 'IDENT' 'BINOP' .

decl_specifiers = type_sc_aux decl1_specifiers .
      decl1_specifiers = decl2_specifiers, *. 
      decl2_specifiers = type_sc decl3_specifiers. 
      decl3_specifiers = decl2_specifiers, *. 

type_sc_aux = type_specifier ,
	      sc_specifier,
	      'IDENT' .

type_sc = type_specifier ,
	   sc_specifier .


type_specifier = 'char' ,
                 'short' ,
                 'int' ,
                 'long' ,
                 'unsigned' ,
                 'void' ,
                 'float' ,
                 'double' ,
		 st_un_specifier,
		 typedef_name .

typedef_name = 'TIPDEF'.

sc_specifier = 'auto' ,
               'static' ,
               'extern' ,
               'register' ,
               'typedef' 21 .

init_decl_list = in_declarator  aux1_ind .

         aux1_ind = ',' init_decl_list , * .

in_declarator = aux_in_dec dclarator [ initializer ] .
	aux_in_dec = type_sc 30 , * .

dclarator = aux1_declarator [ aux2_declarator ] .

	 aux1_declarator = 'IDENT' 1 ,
			   '()' 19 ,
			   '*' 2 dclarator 3 ,
			   '(' 6 init_decl_list 7 ')' 19 .

         aux2_declarator = '()' 19,
			   '[' 5 [ expression ] ']' 27,
			   '(' 6 init_decl_list 7 ')' 19 .

st_un_specifier = aux1_st_un aux2_st_un aux3_st_un .

         aux1_st_un = 'struct' ,
                      'union' .

	 aux2_st_un = 'IDENT', * .

         aux3_st_un = '{' 25 st_decl_list '}' 26 , * .

st_decl_list = st_declaration [ st_decl_list ] .

st_declaration = type_specifier st_list_declarator ';' .

st_list_declarator = st_sdeclarator aux1_slist .

         aux1_slist = ',' st_list_declarator , * .

st_sdeclarator = aux1_sdeclarator, ':' expression .

         aux1_sdeclarator = in_sdeclarator  aux2_sdeclarator .

         aux2_sdeclarator = ':' expression , * .

in_sdeclarator = aux_sin_dec dclarator [ initializer ] .
	aux_sin_dec = type_sc , * .

initializer = '=' 32  aux1_inz  33.
	 aux1_inz = expression , '{'  aux2_inz  '}' .
	 aux2_inz = init_list ,
         ','aux2_inz .

init_list =  aux1_inl ,
	     ',' init_list .

aux1_inl = expression , '{' init_list '}' .

type_name = type_specifier abstract_declarator 22 .

abstract_declarator = aux1_abs [ aux2_abs ] , * .

         aux1_abs = '(' abstract_declarator ')' ,
                    '*' abstract_declarator .

	 aux2_abs = '()' ,
		    '[' [ expression ] ']' .

statement = expression ';' 'EOF' , ';' 'EOF', 'EOF' .

condition = expression cond_aux 'EOF',
	    cond_aux 'EOF' .
	cond_aux=  ';' , * .

expression = aux1_exp [ aux4_exp ] .

	 aux1_exp = primary 18 aux2_exp 29 ,
		    '&' 9 expression 10 ,
			'-' 29  expression ,
			'!'  29 expression ,
			'~' 29 expression ,
		    aux3_exp 29 primary 8 ,
		    'sizeof' 29  expression .

	 aux2_exp = 'ASGNOP' 8 expression ,
		    '=' 8 expression ,
		    '++' 8 ,
	            '--' 8 ,
                    '(' 14 [ expr_list 18 ] ')' 15 ,
		    expression ,
                     * .

         aux3_exp = '++' ,
		    '--' .

         aux4_exp = 'BINOP' 10 expression ,
	            '&' expression ,
		    '?' 10 expression ':' expression ,
		    ',' 10 expression ,
                    '-' 10 expression ,
                    '*' 10 expression .

primary = aux1_pr [ aux3_pr ] .

	 aux1_pr = 'IDENT' 24 ,
		   'CONSTANT' 20 ,
		   'STRING' 20 ,
		   '(' aux2_pr  aux2_exp  ')' ,
		   '*' 12 expression 13 .

	 aux2_pr = expression ,
		   primary ,
		   type_name .

	 aux3_pr = '()' 29 ,
		   '['16 expression ']' 17 ,
		   aux4_pr 'IDENT' .

         aux4_pr = '->' , '.' .

expr_list = expression  expr_aux1 .
	  expr_aux1 = ',' expr_list ,
				   * .


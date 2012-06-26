
UNITRA = [ D ] .

declaration = D 'EOF' .

D      = DIRpre , Extension DA1  DA2 DA3 .
	 DA1      = ED , * .
	 DA2      = LDRini Attribute, * .
	 DA3      = ';' 37 , DA4, * .
	 DA4      = DA5 Ccompo .
	 DA5      = LD , * .

LD     = Dargum [ Dargum ] .

Dargum = Extension ED  DargumA1  Attribute ';' 36 , DIRpre .
         DargumA1 = LDRini , * .

ED     = EDA1 EDA2 .
	 EDA1     = Eclmem , Etipo , Qtipo .
	 EDA2     = ED , * .

Eclmem = 'auto' 13 , 'register' 14, 'static' 15, 'extern' 16, 'typedef' 17 .

Etipo  = 'void' 1 , 'char' 2, 'short' 3, 'int' 4, 'long' 5, 'float' 6,
         'double' 7 , 'signed' 8, 'unsigned' 9, 'TIPDEF' 19, Ees_un  20 , Eenume .

Qtipo  = 'const' 35 , '__const'  35 , 'volatile' 56 .

Ees_un = Ees_unA1  Ees_unA2 12 Ees_unA3  .
	 Ees_unA1 = 'struct' 10, 'union' 11.
	 Ees_unA2 = 'IDENT' 18, 'TIPDEF' 18, * .
	 Ees_unA3 = '{' LDestr '}'  , * .

LDestr = Destru [ Destru ] .

LDRini = DRinic [ ',' 36 LDRiniA1 DRinic ] .
	 LDRiniA1 = ED , * .

DRinic = DR DRinicA1 .
	 DRinicA1 = '=' 49 inicir , * .

Destru = Extension LQE LDRest ';' 25 Attribute, DIRstru .

LQE    = LQEA1 [ LQEA1 ] .
	 LQEA1    = Etipo , Qtipo .

LDRest = DRestr [ ',' 24 DRestr ] .

DRestr = DRestrA1 DRestrA2 .
	 DRestrA1 = DR, * .
	 DRestrA2 = ':' EXcons , * .

Eenume = 'enum' 40 EenumeA1 12 EenumeA2 .
	 EenumeA1 = 'IDENT' 38, * .
	 EenumeA2 = '{' Lenume '}' , * .

Lenume = enumer [ ',' enumer ] .

enumer = 'IDENT' 39 enumerA1 24 .
	 enumerA1 = '=' EXcons , * .

DR     = DRA1 DRdire .
	 DRA1     = aponta , * .

DRdire = DRdireA1 [ DRdireA2 ] .
	 DRdireA1 = 'IDENT' 38 , '(' DR ')' 28.
	 DRdireA2 = '[' DRdireA3 ']' 21 , '('  22 DRdireA4 ')' 29.
	 DRdireA3 = EXcons ,  * .
	 DRdireA4 = Lparam , Lident , * .

aponta = '*' Restrict 23 apontaA1 apontaA2.
	 apontaA1 = LQtipo , * .
	 apontaA2 = aponta , * .

LQtipo = Qtipo [ Qtipo ] .

Lparam = Dparam [ ','  LparamA1 ] .
         LparamA1 = '...' , Dparam .

Dparam = ED DRabst 26 .

Lident = 'IDENT' 48 LidentA1 LidentA2 .
	 LidentA1 = [ ',' 'IDENT' 48 ] .
	 LidentA2 = ';' , * .

inicir = '{' Linicr inicirA1 '}' , EXatri, * .
	 inicirA1 = ',' , * .

Linicr = inicir [ ',' inicir ] .

nomtip = LQE DRabst .

DRabst = DRabstA1 DRabdi .
         DRabstA1 = aponta , * .

DRabdi = DRabdiA1 [ DRabdiA2 ] .
         DRabdiA1 = 'IDENT' 38, '(' DRabdiA3 ')' , * .
	 DRabdiA3 = Lparam , DRabst 27 , * .
         DRabdiA2 = '[' 21 DRabdiA4 ']' , '(' DRabdiA5 ')' .
         DRabdiA4 = EXcons , * .
	 DRabdiA5 = Lparam , Lident , * .

C      = Ccompo , Cselec , Citera , Csalto , Crotul , CEX , ';'  , DIRpre .

Crotul = 'case' EXcons ':'  C , 'default' ':'  C .

statement =  CEX  'EOF', ';' 'EOF', 'return' CsaltoA1 32 ';'.

CEX    = CEXA1 CEXA2 .
	 CEXA1    = EX .
	 CEXA2    = ';'  , ':'  C, 'EOF' .

Ccompo = '{'  CcompoA1 CcompoA2 '}'  .
	 CcompoA1 = LD , * .
	 CcompoA2 = LC , * .

LC     = C [ C ] .

Cselec = 'if'  '(' EX 32 ')' C CselecA1 , 'switch' '(' EX 32 ')' C .
	 CselecA1 = 'else' C , * .

Citera = 'while' '(' EX 32 ')' C ,
	 'do' C 'while' '(' EX 32 ')' ';' ,
	 'for' '('  CiteraA1 ';'  CiteraA1 ';'  CiteraA1  ')'  C .
	 CiteraA1 = EX 32 , * .


Csalto = 'goto'  'IDENT'  ';'  , 'continue' ';'  ,
	 'break' ';'  , 'return' CsaltoA1 ';'  .
	 CsaltoA1 = EX 32 , * .

expression = EX 32 EXA1 , ';' 'EOF', 'EOF'.
             EXA1 = 'EOF', ';' 'EOF'.

EX     = EXatri [ ','  EXatri ] .

EXatri = Extension EXcond EXatriA1 61.
	 EXatriA1 = Oatrib 53 EXatri 54 , * .

Oatrib = '=' 33, '*=' 34, '/=' 34, '%=' 34, '+=' 34, '-=' 34,
	 '<<=' 34, '>>=' 34, '&=' 34, '^=' 34, '|=' 34.

EXcond = EXoulo EXcondA1 61.
	 EXcondA1 = '?' 32 EX  32 ':' EXcond , * .

EXcons = EXcond .

EXoulo = EXelog [ '||'  61 EXelog 66 ] .

EXelog = EXouin [ '&&'  61 EXouin 66 ] .

EXouin = EXouex [ '|' 61 EXouex 58 ] .

EXouex = EXe [ '^' 61 EXe 58 ] .

EXe    = EXigua [ '&' 61 EXigua 58 ] .

EXigua = EXrela [ EXiguaA1 EXrela 66 ] .
	 EXiguaA1 = '==' 61, '!=' 61.

EXrela = EXdesl [ EXrelaA1 EXdesl 66 ] .
	 EXrelaA1 = '<' 61, '>' 61, '<='  61, '>='  61.

EXdesl = EXadit [ EXdeslA1 EXadit 58 ] .
	 EXdeslA1 = '<<' 61 , '>>'  61.

EXadit = EXmult [ EXaditA1 EXmult 58 ] .
	 EXaditA1 = '+'  61, '-'  61.

EXmult = EXunar [ EXmultA1 EXunar 58 ] .
	 EXmultA1 = '*'  61, '/'  61, '%'  61.

EXunar = EXunarA1 [ EXunarA2 ] .
	 EXunarA1 = 'sizeof' 60 '(' 63 EXsizeof ')' 64, Ounari EXunar 47, 
                    EXposf .
	 EXsizeof = nomtip 55, EXunar .
	 EXunarA2 = '++' 34, '--' 34.

Ounari = '&' 51, '*' 44, '+' 60, '-' 60, '~' 60, '!' 60, '++' 45, '--' 46.

EXposf = EXprim [ EXposfA1 ] .
	 EXposfA1 ='[' 63 EX 32 ']' 50 , '(' 57 EXposfA2 59 ')' ,
                   '.' 63 'IDENT' 43 ,
		   '->' 63 'IDENT' 42, '++' 34, '--' 34.
	 EXposfA2 = LEXarg , * .

LEXarg = EXatri 52 [ ',' EXatri 52 ] .

EXprim = 'IDENT' 41  , 'CONSTANT' 55 , '(' 60 EXprimA1 .
	 EXprimA1 = nomtip ')' 64 EXunar , EX ')' 64 .

DIRpre = '#'  DIRpreA1 .
	 DIRpreA1 = DIRinc  , DIRdef  , DIRund  , DIRlin  ,
		    DIRerr  , DIRpra  , DIRcon  , DIRarq  , * .

DIRinc = 'include'  DIRincA1 .
	 DIRincA1 = '<' 'IDENT' DIRincA2 '>' , 'CONSTANT'  , 'SEQCOD' .
	 DIRincA2 = '.' 'IDENT' , * .

DIRdef = 'define'  'SEQCOD' .
	 
DIRund = 'undef' 'IDENT' .

DIRlin = 'line' 'CONSTANT' DIRlinA1 .
	 DIRlinA1 = 'CONSTANT' , * .

DIRerr = 'error'  DIRerrA1 .
	 DIRerrA1 = 'SEQCOD' , * .

DIRpra = 'pragma'  DIRpraA1 .
	 DIRpraA1 = 'SEQCOD' , * .

DIRarq = 'CONSTANT'  'CONSTANT'  [ 'CONSTANT'  ] .

DIRstru = '#' 'CONSTANT' 'CONSTANT' [ 'CONSTANT' ] .

DIRcon = LNif T Pelif Pelse 'endif' .

LNif   = 'if' EXcons , 'ifdef' 'IDENT' , 'ifndef' 'IDENT' .

T      = [ TA1 ] .
	 TA1      = C , D .

Pelif  = [ PelifA1 ] .
	 PelifA1  = 'elif' EXcons T .

Pelse  = [ PelseA1 ] .
	 PelseA1  = 'else' T .

Attribute = '__attribute__' '(' '('  DeffAttr ')' ')', *.

DeffAttr = Attr [ ',' Attr ].

Attr = 'IDENT' IdentCompl, 'CONSTANT', 'const' .

IdentCompl = '(' DeffAttr ')' , * .

Extension = '__extension__' , * .

Restrict = '__restrict', '__restrict', * .

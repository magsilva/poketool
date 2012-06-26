
UNITRA = [ D ] .

D      = DIRpre , Extension DA1  DA2 DA3 48.
	 DA1      = ED , * .
	 DA2      = LDRini Attribute 2 , * .
	 DA3      =  ';' 42 , DA4 .
	 DA4      = DA5 Ccompo 43 .
	 DA5      = LD , * .

LD     = Dargum [ Dargum ] .

Dargum = Extension ED  DargumA1 Attribute ';' 1 , DIRpre .
         DargumA1 = LDRini , * .

ED     = EDA1 EDA2 .
	 EDA1     = Eclmem , Etipo 44, Qtipo .
	 EDA2     = ED , * .

Eclmem = 'auto' , 'register' , 'static' , 'extern' , 'typedef' 29 .

Etipo  = 'void' , 'char' , 'short' , 'int' , 'long' , 'float' , 'double' ,
	 'signed' , 'unsigned' , 'TIPDEF', Ees_un  , Eenume .

Qtipo  = 'const' , '__const', '__inline', 'volatile' , AttA1 .

Ees_un = Ees_unA1 Ees_unA2 Ees_unA3  .
	 Ees_unA1 = 'struct' , 'union' .
	 Ees_unA2 = 'IDENT', 'TIPDEF'  , * .
	 Ees_unA3 = '{'  LDestr  '}'  , * .
                   

LDestr = Destru [ Destru ] .

LDRini = DRinic [ ',' LDRiniA1 DRinic ] .
	 LDRiniA1 = ED , * .

DRinic = DR DRinicA1 .
	 DRinicA1 = '=' inicir , * .

Destru = Extension LQE LDRest ';'  Attribute, DIRstru .

LQE    = LQEA1 [ LQEA1 ] .
	 LQEA1    =  Etipo , Qtipo .


LDRest = DRestr [ ',' DRestr ] .

DRestr = DRestrA1 DRestrA2 .
	 DRestrA1 = DR, * .
	 DRestrA2 = ':' EXcons , * .

Eenume = 'enum' EenumeA1 EenumeA2 .
	 EenumeA1 = 'IDENT' , * .
	 EenumeA2 = '{' Lenume '}' , * .

Lenume = enumer [ ',' enumer ] .

enumer = 'IDENT' enumerA1 .
	 enumerA1 = '=' EXcons , * .

DR     = DRA1 DRdire .
	 DRA1     =  aponta , * .

DRdire = DRdireA1 [ DRdireA2 ] .
	 DRdireA1 = 'IDENT' 39 , '(' 40 DR ')' .
	 DRdireA2 = '[' DRdireA3 ']' , '(' 40 DRdireA4 ')' .
	 DRdireA3 = EXcons ,  * .
	 DRdireA4 = Lparam , Lident , * .

aponta = '*' Restrict apontaA1 apontaA2.
	 apontaA1 = LQtipo , * .
	 apontaA2 = aponta , * .

LQtipo = Qtipo [ Qtipo ] .

Lparam = Dparam [ ',' LparamA1 ] .
         LparamA1 = '...' , Dparam .

Dparam = ED DRabst .

Lident = 'IDENT' LidentA1 LidentA2 .
	 LidentA1 = [ ',' 'IDENT' ] .
	 LidentA2 = ';' , * .

inicir = '{' Linicr inicirA1 '}' , EXatri, * .
	 inicirA1 = ',' , * .

Linicr = inicir [ ',' inicir ] .

nomtip = LQE DRabst .

DRabst = DRabstA1 DRabdi .
         DRabstA1 = aponta , * .

DRabdi = DRabdiA1 [ DRabdiA2 ] .
         DRabdiA1 = 'IDENT' , '(' DRabdiA3 ')' , * .
	 DRabdiA3 = Lparam , DRabst , * .
         DRabdiA2 = '[' DRabdiA4 ']' , '(' DRabdiA5 ')' .
         DRabdiA4 = EXcons , * .
	 DRabdiA5 = Lparam , Lident , * .

C      = Ccompo , Cselec , Citera , Csalto , Crotul , CEX , ';' 8 , DIRpre  .

Crotul = 'case' EXcons ':' 6 C , 'default' ':' 7 C .

CEX    = CEXA1 CEXA2 .
	 CEXA1    = EX .
	 CEXA2    = ';' 8 , ':' 24 C .

Ccompo = '{' 3 CcompoA1 CcompoA2 '}' 4 .
	 CcompoA1 = LD , * .
	 CcompoA2 = LC , * .

LC     = C [ C ] .

Cselec = 'if' 9 '(' EX ')' 10 C CselecA1 , 'switch' 12 '(' EX ')' 13 C .
	 CselecA1 = 'else' 11 C , * .

Citera = 'while' 14 '(' EX ')' 15 C ,
	 'do' 16 C 'while' 17 '(' EX ')' ';' 18 ,
	 'for' 19 '(' 36 CiteraA1 ';' 8 CiteraA1 ';' 21 CiteraA1 20 ')' 36 C .
	 CiteraA1 = EX 22 , * .


Csalto = 'goto' 23 'IDENT' 5 ';' 24 , 'continue' ';' 25 ,
	 'break' ';' 26 , 'return' CsaltoA1 ';' 27 .
	 CsaltoA1 = EX , * .

EX     = EXatri [ ','  EXatri ] .

EXatri = Extension EXcond EXatriA1 .
	 EXatriA1 = Oatrib EXatri , * .

Oatrib = '=' , '*=' , '/=' , '%=' , '+=' , '-=' ,
	 '<<=' , '>>=' , '&=' , '^=' , '|=' .

EXcond = EXoulo EXcondA1 .
	 EXcondA1 = '?' EX ':' EXcond , * .

EXcons = EXcond .

EXoulo = EXelog [ '||' 28 EXelog ] .

EXelog = EXouin [ '&&' 28 EXouin ] .

EXouin = EXouex [ '|' EXouex ] .

EXouex = EXe [ '^' EXe ] .

EXe    = EXigua [ '&' EXigua ] .

EXigua = EXrela [ EXiguaA1 EXrela ] .
	 EXiguaA1 = '==' , '!=' .

EXrela = EXdesl [ EXrelaA1 EXdesl ] .
	 EXrelaA1 = '<' , '>' , '<=' , '>=' .

EXdesl = EXadit [ EXdeslA1 EXadit ] .
	 EXdeslA1 = '<<' , '>>' .

EXadit = EXmult [ EXaditA1 EXmult ] .
	 EXaditA1 = '+' , '-' .

EXmult = EXunar [ EXmultA1 EXunar ] .
	 EXmultA1 = '*' , '/' , '%' .

EXunar = EXunarA1 [ EXunarA2 ] .
	 EXunarA1 = 'sizeof' '(' EXsizeof ')', Ounari EXunar , EXposf .
	 EXsizeof = nomtip , EXunar .
	 EXunarA2 = '++' , '--' .

Ounari = '&' , '*' , '+' , '-' , '~' , '!' , '++' , '--' .

EXposf = EXprim [ EXposfA1 ] .
	 EXposfA1 = '[' EX ']' , '(' EXposfA2 ')' , '.' 'IDENT' ,
		    '->' 'IDENT' , '++' , '--' .
	 EXposfA2 = LEXarg , * .

LEXarg = EXatri [ ',' EXatri ] .

EXprim = 'IDENT' 5 , 'CONSTANT' , '(' EXprimA1 .
	 EXprimA1 = nomtip ')' EXunar , EX ')' .

DIRpre = '#' 35 DIRpreA1 .
	 DIRpreA1 = DIRinc 30 , DIRdef 30 , DIRund 30 , DIRlin 30 ,
		    DIRerr 30 , DIRpra 30 , DIRcon 30 , DIRarq 46 , * .

DIRinc = 'include' 31 DIRincA1 .
	 DIRincA1 = '<' 'IDENT' DIRincA2 '>' , 'CONSTANT' 34 , 'SEQCOD' .
	 DIRincA2 = '.' 'IDENT' , * .

DIRdef = 'define' 31 'SEQCOD' .
	 
DIRund = 'undef' 'IDENT' .

DIRlin = 'line' 'CONSTANT' DIRlinA1 .
	 DIRlinA1 = 'CONSTANT' , * .

DIRerr = 'error' 31 DIRerrA1 .
	 DIRerrA1 = 'SEQCOD' , * .

DIRpra = 'pragma' 31 DIRpraA1 .
	 DIRpraA1 = 'SEQCOD' , * .

DIRarq = 'CONSTANT' 38 'CONSTANT' 41 [ 'CONSTANT' 45 ] .

DIRstru = '#' 'CONSTANT' 38 'CONSTANT' [ 'CONSTANT' ] .

DIRcon = LNif T Pelif Pelse 'endif' .

LNif   = 'if' EXcons , 'ifdef' 'IDENT' , 'ifndef' 'IDENT' .

T      = [ TA1 ] .
	 TA1      = C , D .

Pelif  = [ PelifA1 ] .
	 PelifA1  = 'elif' EXcons T .

Pelse  = [ PelseA1 ] .
	 PelseA1  = 'else' T .

Attribute = [ AttA1 ] , * .

AttA1  = AttA2 AttA3 .

AttA2 = '__attribute__', '__asm__' .

AttA3 = '(' AttA4 ')' .

AttA4 = DeffAttr , '('  DeffAttr ')' .

DeffAttr = Attr [ ',' Attr ] .

Attr = 'IDENT' IdentCompl, AttConst, 'const' , '__const' .

AttConst = 'CONSTANT' [ 'CONSTANT' ] .

IdentCompl = '(' DeffAttr ')' , * .

Extension = '__extension__' , '__inline__' 47, * .

Restrict = '__restrict', '__restrict', * .










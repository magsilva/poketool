
/*
// hpudcam.h			%I%	%Y%	%E% %U%
*/

#ifndef _H_PDUCAM

#define _H_PDUCAM

#ifndef _H_ROTSEM

#include "hrotsem.h"

#endif

#define PR 'r'
#define PFI 'f'
#define PRIM_REP '$'
#define MARK -10
#define UNMARK -20
#define E_REPETIDO 'R'
#define NAO_REPETIDO 'N'
#define ON 1
#define OFF 0


/* Declaracao do tipo de dado que aglomera as informacoes para construcao */
/* dos Descritores */

typedef struct list_bit {
     b_vector conj;
     struct list_bit * next;
     } LISTBITVECTOR;

typedef struct arc_prim {
     PAIRINT arco;
     LISTBITVECTOR * deff_ptr;
     struct arc_prim * next;
     } ARCPRIM;

typedef struct info_descritores {
     b_vector Ni;   /* nos que compoem o grafo PROTO (( i)) */
     b_vector Nt;   /* nos terminais do grafo PROTO (( i)) */
     ARCPRIM * prim_arcs; /* lista de arcos primitivos */
     NOGRAFOI * grafo_i;  /* apontador para o grafo PROTO (( i)) */
    } INFODESCRITORES;

/* Declaracao das Funcoes que Utilizam esse Header */

INFODESCRITORES * det_pot_du_cam PROTO (( struct grafo *, char *, int * )); 
struct no * det_conodef PROTO (( struct no * )); 
void atualiza_pil_cam PROTO (( int, STACK * )); 
int pertence_pil_cam PROTO (( int, STACK * )); 
int existe_Nd_pil_cam PROTO (( STACK *, b_vector * )); 
void imprime_int_stack PROTO (( STACK *, FILE * )); 
LIST * lista_sucessores PROTO (( int, struct grafo * )); 
INFODFNO head PROTO (( LIST ** )); 
void grfi_inicializacao PROTO (( NOGRAFOI **, INFODFNO * )); 
NOGRAFOI * insere_grafo_i PROTO (( INFODFNO *, INFODFNO *, NOGRAFOI *, struct grafo *, char )); 
void liga_grafo_i PROTO (( INFODFNO *, INFODFNO *, NOGRAFOI *, struct grafo * )); 
int existe_no_igual PROTO (( INFODFNO *, NOGRAFOI * )); 
void ini_info_descritores PROTO (( INFODESCRITORES * )); 
void pot_du_cam_descritores PROTO (( INFODESCRITORES * )); 
int belongs_pair_stack PROTO (( PAIRINT *, STACKPAIR * )); 
void reorganiza_grafo PROTO (( NOGRAFOI *, NOGRAFOI ** )); 
void suc_mark PROTO (( NOGRAFOI * )); 
void suc_elimina PROTO (( SUCGRAFOI * )); 
void pred_elimina PROTO (( LISTPRED * )); 
void faca_pred_pfi PROTO (( INFODFNO *, NOGRAFOI * )); 
void faca_arco_pfi PROTO (( INFODFNO *, INFODFNO *, NOGRAFOI * )); 
NOGRAFOI * find_address_grafo_i PROTO (( INFODFNO *, NOGRAFOI * )); 
int no_im_existe_disjunto PROTO (( INFODFNO *, NOGRAFOI * )); 
INFODFNO find_disjunto_no_imagem PROTO (( INFODFNO *, NOGRAFOI * )); 
int esta_deff_contido PROTO (( INFODFNO *, NOGRAFOI * )); 
INFODFNO find_no_imagem PROTO (( INFODFNO *, NOGRAFOI * )); 
NOGRAFOI * cal_nos_contidos PROTO (( INFODFNO *, NOGRAFOI * )); 
NOGRAFOI ret_grafo_i_elem PROTO (( NOGRAFOI ** )); 
void det_arcos_pr PROTO (( int, int, struct grafo *, STACKPAIR * )); 
void info_pfi_elimination PROTO (( INFODFNO *, NOGRAFOI *, struct grafo * )); 
void imprime_descritor PROTO (( STACKARC *, FILE *, INFODESCRITORES * )); 
void imprime_caminho PROTO (( STACKARC *, FILE *, int )); 
void pu_imprime_descritor PROTO (( STACKARC *, FILE *, FILE *, INFODESCRITORES * )); 
void pu_imprime_caminho PROTO (( INFODESCRITORES *, STACKARC *, FILE *, int )); 
void pes_grfi PROTO (( NOGRAFOI *,INFODESCRITORES *, STACKARC *, struct grafo *, FILE *, FILE * )); 
void des_pdu_gen PROTO (( INFODESCRITORES *, struct grafo *, FILE *,FILE * )); 
void des_pu_gen PROTO (( INFODESCRITORES *, FILE *,FILE *, FILE * )); 
int all_pred_marked PROTO (( LISTPRED * )); 
int e_pfi PROTO (( LISTPRED *, NOGRAFOI * )); 
void ins_arc PROTO (( ARCPRIM * *, PAIRINT *, b_vector * )); 
int deff_e_igual PROTO (( LISTBITVECTOR *, b_vector * )); 
void ins_deff PROTO (( LISTBITVECTOR * *, b_vector * )); 
void lib_prim_list PROTO (( ARCPRIM * ));
void msg_print PROTO (( char * ));
void det_arcs PROTO ((struct grafo *, int,   char *, char * ));
void det_nos PROTO ((int, char *, char * ));
void gera_descr_pdu PROTO ((struct grafo *,INFODESCRITORES *, char *, int));
void gera_descr_pu PROTO ((struct grafo *,INFODESCRITORES *, char *, int));
void libera_grfi PROTO ((INFODESCRITORES *, int));
void all_uses(struct grafo * ,char *,char *,NODEINFO,int,int);
void visit_node_all_uses(FILE *,FILE *,FILE *,FILE *,FILE *,struct grafo *,NODEINFO,int,int,int,l_info **,l_info **,l_info **);
void all_du_paths(struct grafo *,char *,char *,NODEINFO,int,int);
void visit_node_du_paths(FILE *,FILE *,struct grafo *,NODEINFO,b_vector *,int,int,l_info **,int *);

#endif /* _H_PDUCAM */





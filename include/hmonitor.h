/*
* Copyright 2012 Marcos Lordello Chaim, José Carlos Maldonado, Mario Jino, 
* CCSL-ICMC <ccsl.icmc.usp.br>
* 
* This file is part of POKE-TOOL.
* 
* POKE-TOOL is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
// hmonitor.h 	%I%	%Y%	%E%	%U%
*/

void escreve_md PROTO (( struct symbol *,int )); 
void ajusta_formato PROTO (( int )); 
void ajusta_ponta_de_prova PROTO (( int )); 
void write_no PROTO (( int )); 
void for_write_no PROTO (( int )); 
void in_while_monitor PROTO (( int )); 
void fim_while_monitor PROTO (( int )); 
void in_for_monitor PROTO (( int )); 
void meio_for_monitor PROTO (( int )); 
void fim_for_monitor PROTO (( int )); 
void in_until_monitor PROTO (( int )); 
void meio_until_monitor PROTO (( int )); 
void fim_until_monitor PROTO (( int )); 
void in_case_monitor PROTO (( int )); 
void fim_case_monitor PROTO (( void )); 
void in_std_monitor PROTO (( int )); 
void fim_std_monitor PROTO (( int )); 
void fecha_chave_monitor PROTO (( int )); 
void in_return_monitor PROTO (( int )); 
void fim_return_monitor PROTO (( int )); 
void std_monitor PROTO (( int )); 
void meio2_for_monitor PROTO (( int )); 
void break_monitor PROTO (( int )); 
void finaliza_monitoracao PROTO (( void )); 
void meio_monitoracao PROTO (( void )); 
void inicia_monitoracao PROTO (( void )); 
void writeIniciador PROTO (( int )); 
void writeFinalizador PROTO (( int )); 
void last_node_write PROTO (( int )); 
void cont_monitor PROTO (( int )); 
void atualiza_num_for PROTO (( int,int )); 
void info_no_clear PROTO ((void));
void info_no_in PROTO ((void));
void info_no_b_vector_ajust(void);
void cria_include(void);

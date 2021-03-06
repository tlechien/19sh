/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 07:12:40 by aben-azz          #+#    #+#             */
/*   Updated: 2019/11/12 20:06:52 by tlechien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROMPT_H
# define PROMPT_H
# include <term.h>
# include <termios.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/uio.h>
# include <unistd.h>
# include <unistd.h>
# include <math.h>
# include <fcntl.h>
# include <dirent.h>
# include <curses.h>
# include <sys/ioctl.h>
# define PROMPT1 			"\x1b[0m\x1b[32m\x1b[1m➜  \x1b[0m"
# define PROMPT1_ERR 		"\x1b[0m\x1b[31m\x1b[1m➜  \x1b[0m"
# define PROMPT2 			"\x1b[36m\x1b[1m%s \x1b[0m"
# define PROMPT3 			"\x1b[1m\x1b[34mgit:(\x1b[31m%s\x1b[34m) \x1b[0m"
# define PROMPT4 			"\x1b[1m\x1b[31m%s\x1b[0m\x1b[33m\x1b[1m ✗ \x1b[0m"
# define DEFAULT_HISTORY_NAME	".42sh_history"
# define DEFAULT_ENV_FILE_NAME	".42sh_env"
# define DEFAULT_PROMPT_COLOR	"\x1b[37m"
# define BUFFSIZE				4096
# define ARROW_CODE1			27
# define ARROW_CODE2			91
# define UP						65
# define DOWN					66
# define RIGHT					67
# define LEFT					68
# define SHIFT_CODE1			27
# define SHIFT_CODE2			91
# define SHIFT_CODE3			49
# define ALT_CODE1				27
# define ALT_CODE2				27
# define ALT_CODE3				91
# define ARROW_CODE3			59
# define ARROW_CODE4			50
# define SHIFT_UP				75
# define SHIFT_DOWN				76
# define SHIFT_RIGHT			77
# define SHIFT_LEFT				78
# define BACKSPACE				127
# define SPACE					32
# define ENTER					10
# define CTRL_R					18
# define CTRL_D					4
# define TAB					9
# define SHIFT_TAB				90
# define HOME_END1				27
# define HOME_END2				91
# define HOME					72
# define END					70
# define COPY					0
# define CUT					-120
# define PASTE					-102
# define MAX_HISTORY_LENGHT		4096
# define MAX_PATH				PATH_MAX
# define UNUSED					0
# define DEBUG_LOG				0

typedef struct stat			t_stat;
typedef struct termios		t_term;
typedef struct dirent		t_dirent;
typedef struct passwd		t_passwd;
typedef struct group		t_group;
typedef struct				s_built
{
	char		*builtin;
	int			(*function)(char **argv);
}							t_built;
typedef struct				s_cap
{
	char		*up;
	char		*down;
	char		*right;
	char		*left;
	char		*carriage;
	char		*clr_curr_line;
	char		*clr_all_line;
	char		*sound;
	int			cursx;
	int			cursx_max;
	int			cursy_max;
	int			cursy;
	int			prompt_len;
	int			init_len;
	int			char_len;
	char		command[BUFFSIZE];
	int			overflow;
	char		carry[2];
	char		*prompt;
}							t_cap;

typedef struct				s_ctrl_r
{
	int			state;
	int			index;
	int			not_found;
	char		data[BUFFSIZE];
	char		found[BUFFSIZE];
}							t_ctrl_r;

typedef struct				s_cc
{
	int			state;
	int			type;
	int			from;
	int			to;
	char		copied[BUFFSIZE];
}							t_cc;

typedef struct				s_file
{
	t_stat		stats;
	char		name[MAX_PATH];
	char		path[MAX_PATH];
	char		full_path[MAX_PATH];
}							t_file;

typedef struct				s_event
{
	int			key;
	int			(*function)(t_cap *tcap);
}							t_event;
typedef struct				s_tab
{
	int			state;
	int			pos;
	int			len;
	int			carry;
	int			row;
	int			col;
	int			max_offset;
	char		match[BUFFSIZE];
	int			isdir;
	char		after[BUFFSIZE];
	char		comp[MAX_PATH];
	char		*data[MAX_HISTORY_LENGHT];
	char		ext[MAX_HISTORY_LENGHT];
}							t_ab;
typedef struct				s_history
{
	int			len;
	int			read;
	char		*file_name;
	int			position;
	int			match[BUFFSIZE];
	char		*data[MAX_HISTORY_LENGHT];
}							t_history;

extern t_event g_arrow_event[];
extern t_event g_key_event[];
extern t_event g_alt_event[];

/*
**	ARROW_EVENTS.C
*/

int							arrow_up_event(t_cap *tcap);
int							arrow_down_event(t_cap *tcap);
int							arrow_right_event(t_cap *tcap);
int							arrow_left_event(t_cap *tcap);
void						ft_clear_replace(t_cap *tcap);

/*
**	SHIFT_EVENTS.C
*/

int							shift_arrow_up_event(t_cap *tcap);
int							shift_arrow_down_event(t_cap *tcap);
int							shift_arrow_right_event(t_cap *tcap);
int							shift_arrow_left_event(t_cap *tcap);
int							shift_tab_event(t_cap *tcap);

/*
**	KEY_EVENTS.C
*/

int							enter_event(t_cap *tcap);
int							backspace_event(t_cap *tcap);
int							ctrl_r_event(t_cap *tcap);
int							tab_event(t_cap *tcap);
int							ctrl_d_event(t_cap *tcap);
int							space_event(t_cap *tcap);

/*
**	HOME_END_EVENTS.C
*/

int							home_event(t_cap *tcap);
int							end_event(t_cap *tcap);

/*
**	COPY_CUT.C
*/

int							ft_copy(t_cap *tcap);
int							ft_cut(t_cap *tcap);
int							ft_paste(t_cap *tcap);

/*
**	VAR.C
*/

int							read_var(char **var);
char						*get_string_var(char *string, char **var);
int							get_int_var(char *string, char **var);

/*
**	READER.C
*/

int							read_buffer(char *buffer, t_cap *tcap);
void						ft_clear_all_lines(t_cap *tcap);

/*
** PROMPT_PREFIX.c
*/

void						print_prompt_prefix(void);

/*
** CTRL_R.c
*/

int							add_buffer_ctrl_r(char *buffer, t_ctrl_r *ctrl_r);
int							back_i_search(t_ctrl_r *ctrl_r, t_cap *tcap);
int							clear_before_ctrl_r(t_cap *tcap, t_ctrl_r *ctrl_r);
int							end_ctrl_r(t_ctrl_r *ctrl_r);

/*
**	OTHERS.C
*/

int							ft_put_termcaps(int c);
char						*get_git_status(void);
int							ft_clean_eol(t_cap *tcap);

/*
**	SIGNAL_HANDLER.C
*/

void						sigfork(int sig);
void						sigint_handler(int sig);
void						sigwinch_handler(int sig);
void						sigtstp_handler(int sig);
void						sigtstp_dflhandler(int sig);

/*
**	READ_KEY.C
*/

int							is_key(char key[3]);
int							read_key(char buffer, t_cap *tcap);

/*
**	READ_ARROW.C
*/

char						is_shift_arrow(char key[4]);
int							is_arrow(char key[4]);
int							read_arrow(char buffer, t_cap *tcap);

/*
**	READ_ALT.C
*/

int							is_alt(char *key);

/*
** MOVE.C
*/

void						ft_replace_cursor(t_cap *tcap);
int							ft_left(t_cap *tcap);
int							ft_right(t_cap *tcap);
int							ft_move(t_cap *tcap, char *string, int n);

/*
**	HISTORY.C
*/

int							write_history(char *string, t_history *history);
int							add_cmd_to_history(char *str, t_history *history);
int							history_builtin(int argc, char **argv);
int							read_history(t_history *history);

/*
**HISTO_UP_DOWN.c
*/

int							histo_up(t_cap *tcap, t_history *h);
int							histo_down(t_cap *tcap, t_history *h);

/*
**PUSH.c
*/

int							ft_add_n_char(char *buff, int pos, int len,
																t_cap *tcap);
int							ft_insert(char *buff, t_cap *tcap);
int							ft_delete_n_char(t_cap *tcap, int pos, int len);
int							ft_delete_back(t_cap *tcap);
void						ft_clean_buff(t_cap *tcap);

/*
**COMPLETION.c
*/

int							ft_tab(t_cap *tcap, t_ab *autocomp);

/*
** HISTORY_EXPANSION.c
*/

int							expansion_history(char *string, t_cap *tcap, int i);

/*
**INIT_STRUCT.c
*/

int							init_struct(char **env);
int							init_termcap(t_cap *tcap);

/*
** completion.c
*/
int							init_autocomp(t_cap *tcap, t_ab *autocomp);
char						file_name_ext(char *string, t_stat stats, char *n);
void						cat_fullpath(char *full_path, char *name, char *p);
char						*env_path(char **env);
int							get_argi(t_cap *tc, int position);
int							is_first_argi(t_cap *tc, int position);
void						get_word(t_cap *tc, int position, char *path);
int							is_env_var(t_ab *autocomp, char *path);
void						get_quote(t_ab *autocomp, char *str);
int							is_space_before(t_cap *tcap, int position);
void						create_file(char *name, char *path,
														t_ab *comp, char ext);
void						get_tilde(char *path, char *final_path);
int							add_to_completion(t_ab *autocomp,
														char *path, char ext);
int							env_completion(t_ab *autocomp, char *key);
int							command_completion(t_ab *autocomp, char *key);
int							path_completion(t_ab *autocomp, char *key);
int							arg_completion(t_ab *comp, t_cap *tc,
													char *str, int position);
int							first_arg_completion(t_ab *comp, t_cap *tc,
															char *str, int pos);
int							is_separator(char *s, int position);
int							add_to_completion(t_ab *comp, char *path, char ext);
int							first_arg_completion(t_ab *comp, t_cap *tc,
															char *str, int pos);
int							print_name(t_ab *autocomp, char *str, int i);
int							process_completion(t_ab *autocomp);
int							print_completion(t_ab *autocomp, int row, int i);
int							multi_putstr_fd(char *s1, char *s2, char *s3);
/*
**	MAIN.C
*/

char						*read_line(t_cap *tcap);
int							debug(void);
char						*clean_before_return(t_cap *tcap);

#endif

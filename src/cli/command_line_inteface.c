/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_line_inteface.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:47:45 by bclaeys           #+#    #+#             */
/*   Updated: 2024/11/28 13:14:21 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <readline/history.h>
#include <stdbool.h>
#include <time.h>

int	ms_lex_and_parse(t_var_data *var_data,
						t_error_checks *error_checks,
						char *prompt)
{
	int	i;

	i = 0;
	while (prompt[i] && ft_iswhitespace(prompt[i]))
		i++;
	if (!prompt[0] || !prompt[i])
		return (-1);
	var_data->first_node_lexer = tokenizer(prompt, var_data,
			var_data->first_node_lexer, 0);
	if (error_checks->lexer_level_syntax_error == true)
		return (-1);
	if (!var_data->first_node_lexer)
		return (free(prompt), 1);
	if (parser(&var_data->first_node_ast, var_data->first_node_lexer, var_data))
		return (1);
	if (error_checks->parser_level_syntax_error == true)
		return (-1);
	return (0);
}

void	test_print_parser(t_var_data *var_data)
{
	int				i;
	t_ast_node		*tmp;
	t_ast_redir		*tmp_redir;

	i = 0;
	tmp = var_data->first_node_ast;
	while (tmp)
	{
		ft_printf("command: %s\n", tmp->command);
		while (tmp->arguments && tmp->arguments[i])
		{
			ft_printf("arguments: %s\n", tmp->arguments[i]);
			i++;	
		}
		i = 0;
		tmp_redir = tmp->redirect;
		while (tmp_redir)
		{
			ft_printf("redirects: type = %d, file = %s\n", 
					tmp_redir->type, tmp_redir->file);
			tmp_redir = tmp_redir->next_redir;
		}
		tmp = tmp->pipe;
		ft_printf("+++++\n");
	}
}

int	ms_execute(t_var_data *var_data)
{
	t_ast_node *tmp_node;
	int  		fd;

	tmp_node = var_data->first_node_ast;
	fd = 1; // redir past dit aan
	while (tmp_node)
	{
	// redirs: hier al nieuwe file aanpassen
	// builtins_check
	check_if_builtin(var_data, tmp_node, fd);
	// binary_handler
	// pipe_handler
	tmp_node = tmp_node->pipe;		
	}
	return (1);
}

int	ms_command_line_inteface(t_var_data *var_data)
{
	char			*prompt;
	int				flow_check;

	prompt = NULL;
	while (true)
	{
		big_free(var_data, prompt);
		init_error_data(var_data->error_checks);
		prompt = readline("\033[33mbazzels_minishell> \033[0m");
		add_history(prompt);
		if (!prompt)
			return (ft_printf("exit\n"), 0);
		if (!ft_strncmp(prompt, "exit", 5))
			break ;
		flow_check = ms_lex_and_parse(var_data, var_data->error_checks, prompt);
		if (flow_check == -1)
			continue ;
		else if (flow_check == 1)
			return (1);
		test_print_parser(var_data); //TEST
		ms_execute(var_data);
	}
	return (free(prompt), 0);
}

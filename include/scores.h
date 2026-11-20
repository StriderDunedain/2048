#ifndef SCORES_H
# define SCORES_H

# include <game.h>

typedef struct s_scores
{
	unsigned long long	best_4;
	unsigned long long	best_5;
}t_scores;

/* File format: "4 <unsigned decimal>\n5 <unsigned decimal>\n". */
int	scores_load(t_scores *scores);
int	scores_save(const t_scores *scores);
int	scores_update(t_scores *scores, const t_game *game);

#endif

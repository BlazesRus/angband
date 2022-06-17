/*
 * File: score-ui.h
 * Purpose: Highscore display for Angband
 */

#if (defined(MBandServer)||defined(HybridClient)) && !defined(INCLUDED_SCORE_UI_H)
#define INCLUDED_SCORE_UI_H

extern void do_cmd_knowledge_scores(struct player *p, int line);

#endif /* INCLUDED_SCORE_UI_H */

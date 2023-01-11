
#line 1 "ext/dtext/dtext.rl"
// situationally print newlines to make the generated html
// easier to read
#define PRETTY_PRINT 0

#include "dtext.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

static const size_t MAX_STACK_DEPTH = 512;

typedef enum element_t {
  QUEUE_EMPTY = 0,
  BLOCK_P = 1,
  INLINE_SPOILER = 2,
  BLOCK_SPOILER = 3,
  BLOCK_QUOTE = 4,
  BLOCK_SECTION = 5,
  BLOCK_NODTEXT = 6,
  BLOCK_CODE = 7,
  BLOCK_TD = 8,
  INLINE_NODTEXT = 9,
  INLINE_B = 10,
  INLINE_I = 11,
  INLINE_U = 12,
  INLINE_S = 13,
  INLINE_TN = 14,
  BLOCK_TN = 15,
  BLOCK_TABLE = 16,
  BLOCK_THEAD = 17,
  BLOCK_TBODY = 18,
  BLOCK_TR = 19,
  BLOCK_UL = 20,
  BLOCK_LI = 21,
  BLOCK_TH = 22,
  BLOCK_H1 = 23,
  BLOCK_H2 = 24,
  BLOCK_H3 = 25,
  BLOCK_H4 = 26,
  BLOCK_H5 = 27,
  BLOCK_H6 = 28,
  BLOCK_STRIP = 30,
  INLINE_SUP = 31,
  INLINE_SUB = 32,
  INLINE_COLOR = 33
} element_t;


#line 1038 "ext/dtext/dtext.rl"



#line 60 "ext/dtext/dtext.c"
static const short _dtext_to_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 88, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 88, 
	0, 88, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 88, 0, 88, 0, 88, 0, 88, 
	0, 88, 0, 0, 0, 0, 0
};

static const short _dtext_from_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 89, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 89, 
	0, 89, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 89, 0, 89, 0, 89, 0, 89, 
	0, 89, 0, 0, 0, 0, 0
};

static const int dtext_start = 532;
static const int dtext_first_final = 532;
static const int dtext_error = -1;

static const int dtext_en_basic_inline = 551;
static const int dtext_en_inline = 553;
static const int dtext_en_inline_code = 609;
static const int dtext_en_code = 611;
static const int dtext_en_nodtext = 613;
static const int dtext_en_table = 615;
static const int dtext_en_list = 617;
static const int dtext_en_main = 532;


#line 1041 "ext/dtext/dtext.rl"

static inline void dstack_push(StateMachine * sm, element_t element) {
  g_queue_push_tail(sm->dstack, GINT_TO_POINTER(element));
}

static inline element_t dstack_pop(StateMachine * sm) {
  return GPOINTER_TO_INT(g_queue_pop_tail(sm->dstack));
}

static inline element_t dstack_peek(const StateMachine * sm) {
  return GPOINTER_TO_INT(g_queue_peek_tail(sm->dstack));
}

/*
static inline bool dstack_search(StateMachine * sm, const int * element) {
  return g_queue_find(sm->dstack, (gconstpointer)element);
}
*/

static inline bool dstack_check(const StateMachine * sm, element_t expected_element) {
  return dstack_peek(sm) == expected_element;
}

static inline bool dstack_check2(const StateMachine * sm, element_t expected_element) {
  if (sm->dstack->length < 2) {
    return false;
  }

  element_t top2 = GPOINTER_TO_INT(g_queue_peek_nth(sm->dstack, sm->dstack->length - 2));
  return top2 == expected_element;
}

static inline void append(StateMachine * sm, bool is_markup, const char * s) {
  if (!is_markup || !sm->f_strip) {
    sm->output = g_string_append(sm->output, s);
  }
}

static inline void append_c(StateMachine * sm, char s) {
  sm->output = g_string_append_c(sm->output, s);
}

static inline void append_c_html_escaped(StateMachine * sm, char s) {
  switch (s) {
    case '<':
      sm->output = g_string_append(sm->output, "&lt;");
      break;

    case '>':
      sm->output = g_string_append(sm->output, "&gt;");
      break;

    case '&':
      sm->output = g_string_append(sm->output, "&amp;");
      break;

    case '"':
      sm->output = g_string_append(sm->output, "&quot;");
      break;

    default:
      sm->output = g_string_append_c(sm->output, s);
      break;
  }
}

static inline void append_segment(StateMachine * sm, bool is_markup, const char * a, const char * b) {
  if (!(is_markup && sm->f_strip)) {
    sm->output = g_string_append_len(sm->output, a, b - a + 1);
  }
}

static inline void append_segment_uri_escaped(StateMachine * sm, const char * a, const char * b) {
  if (sm->f_strip) {
    return;
  }

  g_autofree char * segment1 = NULL;
  g_autofree char * segment2 = NULL;
  g_autoptr(GString) segment_string = g_string_new_len(a, b - a + 1);

  segment1 = g_uri_escape_string(segment_string->str, NULL, TRUE);
  segment2 = g_markup_escape_text(segment1, -1);
  sm->output = g_string_append(sm->output, segment2);
}

static inline void append_segment_uri_possible_fragment_escaped(StateMachine * sm, const char * a, const char * b) {
  if (sm->f_strip) {
    return;
  }

  g_autofree char * segment1 = NULL;
  g_autofree char * segment2 = NULL;
  g_autoptr(GString) segment_string = g_string_new_len(a, b - a + 1);

  segment1 = g_uri_escape_string(segment_string->str, "#", TRUE);
  segment2 = g_markup_escape_text(segment1, -1);
  sm->output = g_string_append(sm->output, segment2);
}

static inline void append_segment_html_escaped(StateMachine * sm, const char * a, const char * b) {
  g_autofree gchar * segment = g_markup_escape_text(a, b - a + 1);
  sm->output = g_string_append(sm->output, segment);
}

static inline void append_link(StateMachine * sm, const char * title, const char * ahref) {
  append(sm, true, ahref);
  append_segment_uri_escaped(sm, sm->a1, sm->a2 - 1);
  append(sm, true, "\">");
  append(sm, false, title);
  append_segment_html_escaped(sm, sm->a1, sm->a2 - 1);
  append(sm, true, "</a>");
}

static inline void append_url(StateMachine * sm, const char * url_start, const char * url_end, const char * title_start, const char * title_end) {
  append(sm, true, "<a rel=\"nofollow\" class=\"dtext-link\" href=\"");
  append_segment_html_escaped(sm, url_start, url_end);
  append(sm, true, "\">");
  if (sm->f_strip) {
    append_c(sm, ' ');
  }
  append_segment_html_escaped(sm, title_start, title_end);
  append(sm, true, "</a>");
}

static inline bool append_named_url(StateMachine * sm, const char * url_start, const char * url_end, const char * title_start, const char * title_end) {
  g_autoptr(GString) parsed_title = parse_basic_inline(title_start, title_end - title_start, sm->f_strip);

  if (!parsed_title) {
    return false;
  }

  if (url_start[0] == '/' || url_start[0] == '#') {
    append(sm, true, "<a rel=\"nofollow\" class=\"dtext-link\" href=\"");
  } else {
    append(sm, true, "<a rel=\"nofollow\" class=\"dtext-link dtext-external-link\" href=\"");
  }

  append_segment_html_escaped(sm, url_start, url_end);
  append(sm, true, "\">");
  if (sm->f_strip) {
    append_c(sm, ' ');
  }
  append_segment(sm, false, parsed_title->str, parsed_title->str + parsed_title->len - 1);
  append(sm, true, "</a>");

  return true;
}

static inline void append_wiki_link(StateMachine * sm, const char * tag, const size_t tag_len, const char * title, const size_t title_len) {
  g_autofree gchar* lowercased_tag = g_utf8_strdown(tag, tag_len);
  g_autoptr(GString) normalized_tag = g_string_new(g_strdelimit(lowercased_tag, " ", '_'));

  if (tag[0] == '#') {
    append(sm, true, "<a rel=\"nofollow\" class=\"dtext-link dtext-wiki-link\" href=\"#");
    append_segment_uri_escaped(sm, lowercased_tag+1, lowercased_tag + tag_len - 1);
    append(sm, true, "\">");
  } else {
    append(sm, true, "<a rel=\"nofollow\" class=\"dtext-link dtext-wiki-link\" href=\"/wiki_pages/show_or_new?title=");
    append_segment_uri_possible_fragment_escaped(sm, normalized_tag->str, normalized_tag->str + normalized_tag->len - 1);
    append(sm, true, "\">");
  }
  append_segment_html_escaped(sm, title, title + title_len - 1);
  append(sm, true, "</a>");
}

static inline void append_post_search_link(StateMachine * sm, const char * tag, const size_t tag_len, const char * title, const size_t title_len) {
  g_autofree gchar* lowercased_tag = g_utf8_strdown(tag, tag_len);
  g_autoptr(GString) normalized_tag = g_string_new(lowercased_tag);

  append(sm, true, "<a rel=\"nofollow\" class=\"dtext-link dtext-post-search-link\" href=\"/posts?tags=");
  append_segment_uri_escaped(sm, normalized_tag->str, normalized_tag->str + normalized_tag->len - 1);
  append(sm, true, "\">");
  append_segment_html_escaped(sm, title, title + title_len - 1);
  append(sm, true, "</a>");
}

static inline void append_paged_link(StateMachine * sm, const char * title, const char * ahref, const char * param) {
  append(sm, true, ahref);
  append_segment(sm, true, sm->a1, sm->a2 - 1);
  append(sm, true, param);
  append_segment(sm, true, sm->b1, sm->b2 - 1);
  append(sm, true, "\">");
  append(sm, false, title);
  append_segment(sm, false, sm->a1, sm->a2 - 1);
  append(sm, false, "/p");
  append_segment(sm, false, sm->b1, sm->b2 - 1);
  append(sm, true, "</a>");
}

static inline void append_block_segment(StateMachine * sm, const char * a, const char * b) {
  if (sm->f_inline) {
    // sm->output = g_string_append_c(sm->output, ' ');
  } else if (sm->f_strip) {
    if (sm->output->len > 0 && sm->output->str[sm->output->len-1] != ' ') {
      append_c(sm, ' ');
    }
  } else {
    sm->output = g_string_append_len(sm->output, a, b - a + 1);
  }
}

static inline void append_block(StateMachine * sm, const char * s) {
  append_block_segment(sm, s, s + strlen(s) - 1);
}

static void append_closing_p(StateMachine * sm) {
  size_t i = sm->output->len;

  if (i > 4 && !strncmp(sm->output->str + i - 4, "<br>", 4)) {
    sm->output = g_string_truncate(sm->output, sm->output->len - 4);
  }

  if (i > 3 && !strncmp(sm->output->str + i - 3, "<p>", 3)) {
    sm->output = g_string_truncate(sm->output, sm->output->len - 3);
    return;
  }

  append_block(sm, "</p>");
}

static void append_closing_p_if(StateMachine * sm) {
  if (!dstack_check(sm, BLOCK_P)) {
    return;
  }

  dstack_pop(sm);
  append_closing_p(sm);
}

static void dstack_open_inline(StateMachine * sm, element_t type, const char * html) {
  g_debug("push inline element [%d]: %s", type, html);

  dstack_push(sm, type);
  append(sm, true, html);
}

static void dstack_open_block(StateMachine * sm, element_t type, const char * html) {
  g_debug("push block element [%d]: %s", type, html);

  dstack_push(sm, type);
  append_block(sm, html);
}

static void dstack_close_inline(StateMachine * sm, element_t type, const char * close_html) {
  if (dstack_check(sm, type)) {
    g_debug("pop inline element [%d]: %s", type, close_html);

    dstack_pop(sm);
    append(sm, true, close_html);
  } else {
    g_debug("ignored out-of-order closing inline tag [%d]", type);

    append_segment(sm, true, sm->ts, sm->te - 1); // XXX should be false?
  }
}

static bool dstack_close_block(StateMachine * sm, element_t type, const char * close_html) {
  if (dstack_check(sm, type)) {
    g_debug("pop block element [%d]: %s", type, close_html);

    dstack_pop(sm);
    append_block(sm, close_html);
    return true;
  } else {
    g_debug("ignored out-of-order closing block tag [%d]", type);

    append_block_segment(sm, sm->ts, sm->te - 1);
    return false;
  }
}

static void dstack_rewind(StateMachine * sm) {
  element_t element = dstack_pop(sm);

  switch(element) {
    case BLOCK_P: append_closing_p(sm); break;
    case INLINE_SPOILER: append(sm, true, "</span>"); break;
    case BLOCK_SPOILER: append_block(sm, "</div>"); break;
    case BLOCK_QUOTE: append_block(sm, "</blockquote>"); break;
    case BLOCK_SECTION: append_block(sm, "</div></div>"); break;
    case BLOCK_NODTEXT: append_closing_p(sm); break;
    case BLOCK_CODE: append_block(sm, "</pre>"); break;
    case BLOCK_TD: append_block(sm, "</td>"); break;
    case BLOCK_TH: append_block(sm, "</th>"); break;

    case INLINE_NODTEXT: break;
    case INLINE_B: append(sm, true, "</strong>"); break;
    case INLINE_I: append(sm, true, "</em>"); break;
    case INLINE_U: append(sm, true, "</u>"); break;
    case INLINE_S: append(sm, true, "</s>"); break;
    case INLINE_SUB: append(sm, true, "</sub>"); break;
    case INLINE_SUP: append(sm, true, "</sup>"); break;
    case INLINE_COLOR: append(sm, true, "</span>"); break;
    case INLINE_TN: append(sm, true, "</span>"); break;

    case BLOCK_TN: append_closing_p(sm); break;
    case BLOCK_TABLE: append_block(sm, "</table>"); break;
    case BLOCK_THEAD: append_block(sm, "</thead>"); break;
    case BLOCK_TBODY: append_block(sm, "</tbody>"); break;
    case BLOCK_TR: append_block(sm, "</tr>"); break;
    case BLOCK_UL: append_block(sm, "</ul>"); break;
    case BLOCK_LI: append_block(sm, "</li>"); break;
    case BLOCK_H6: append_block(sm, "</h6>"); break;
    case BLOCK_H5: append_block(sm, "</h5>"); break;
    case BLOCK_H4: append_block(sm, "</h4>"); break;
    case BLOCK_H3: append_block(sm, "</h3>"); break;
    case BLOCK_H2: append_block(sm, "</h2>"); break;
    case BLOCK_H1: append_block(sm, "</h1>"); break;
    case BLOCK_STRIP: append_c(sm, ' '); break;

    case QUEUE_EMPTY: break;
  }
}

static void dstack_close_before_block(StateMachine * sm) {
  while (1) {
    if (dstack_check(sm, BLOCK_P)) {
      dstack_pop(sm);
      append_closing_p(sm);
    } else if (dstack_check(sm, BLOCK_LI) || dstack_check(sm, BLOCK_UL)) {
      dstack_rewind(sm);
    } else {
      return;
    }
  }
}

static void dstack_close(StateMachine * sm) {
  while (!g_queue_is_empty(sm->dstack)) {
    dstack_rewind(sm);
  }
}

static void dstack_close_list(StateMachine * sm) {
  while (dstack_check(sm, BLOCK_LI) || dstack_check(sm, BLOCK_UL)) {
    dstack_rewind(sm);
  }

  sm->list_mode = false;
  sm->list_nest = 0;
}

// Returns the preceding non-boundary character if `c` is a boundary character.
// Otherwise, returns `c` if `c` is not a boundary character. Boundary characters
// are trailing punctuation characters that should not be part of the matched text.
static inline const char* find_boundary_c(const char* c) {
  gunichar ch = g_utf8_get_char(g_utf8_prev_char(c + 1));
  int offset = 0;

  // Close punctuation: http://www.fileformat.info/info/unicode/category/Pe/list.htm
  // U+3000 - U+303F: http://www.fileformat.info/info/unicode/block/cjk_symbols_and_punctuation/list.htm
  if (g_unichar_type(ch) == G_UNICODE_CLOSE_PUNCTUATION || (ch >= 0x3000 && ch <= 0x303F)) {
    offset = g_unichar_to_utf8(ch, NULL);
  }

  switch (*c) {
    case ':':
    case ';':
    case '.':
    case ',':
    case '!':
    case '?':
    case ')':
    case ']':
    case '<':
    case '>':
      offset = 1;
  }

  return c - offset;
}

/*
static bool print_machine(StateMachine * sm) {
  printf("p=%c\n", *sm->p);
  return true;
}
*/

StateMachine* init_machine(const char * src, size_t len, bool f_strip, bool f_inline, bool f_mentions, bool f_color, long f_max_thumbs) {
  size_t output_length = 0;
  StateMachine* sm = (StateMachine *)g_malloc0(sizeof(StateMachine));

  sm->p = src;
  sm->pb = sm->p;
  sm->pe = sm->p + len;
  sm->eof = sm->pe;
  sm->ts = NULL;
  sm->te = NULL;
  sm->cs = dtext_start;
  sm->act = 0;
  sm->top = 0;
  output_length = len;
  if (output_length < (INT16_MAX / 2)) {
    output_length *= 2;
  }
  sm->output = g_string_sized_new(output_length);
  sm->a1 = NULL;
  sm->a2 = NULL;
  sm->b1 = NULL;
  sm->b2 = NULL;
  sm->f_inline = f_inline;
  sm->f_strip = f_strip;
  sm->f_mentions = f_mentions;
  sm->allow_color = f_color;
  sm->thumbnails_left = f_max_thumbs < 0 ? 5000 : f_max_thumbs; // Cap for sanity even if "unlimited"
  sm->posts = g_array_sized_new(FALSE, TRUE, sizeof(long), 10);
  sm->stack = g_array_sized_new(FALSE, TRUE, sizeof(int), 16);
  sm->dstack = g_queue_new();
  sm->error = NULL;
  sm->list_nest = 0;
  sm->list_mode = false;
  sm->header_mode = false;
  sm->d = 0;
  sm->b = 0;
  sm->quote = 0;

  return sm;
}

void free_machine(StateMachine * sm) {
  g_string_free(sm->output, TRUE);
  g_array_unref(sm->stack);
  g_array_unref(sm->posts);
  g_queue_free(sm->dstack);
  g_clear_error(&sm->error);
  g_free(sm);
}

GQuark dtext_parse_error_quark() {
  return g_quark_from_static_string("dtext-parse-error-quark");
}

GString* parse_basic_inline(const char* dtext, const ssize_t length, const bool f_strip) {
    GString* output = NULL;
    StateMachine* sm = init_machine(dtext, length, f_strip, true, false, false, 0);
    sm->cs = dtext_en_basic_inline;

    if (parse_helper(sm)) {
      output = g_string_new(sm->output->str);
    } else {
      g_debug("parse_basic_inline failed");
    }

    free_machine(sm);
    return output;
}

gboolean parse_helper(StateMachine* sm) {
  const gchar* end = NULL;

  g_debug("start\n");

  if (!g_utf8_validate(sm->pb, sm->pe - sm->pb, &end)) {
    g_set_error(&sm->error, DTEXT_PARSE_ERROR, DTEXT_PARSE_ERROR_INVALID_UTF8, "invalid utf8 starting at byte %td", end - sm->pb + 1);
    return FALSE;
  }

  
#line 698 "ext/dtext/dtext.c"
	{
	( sm->top) = 0;
	( sm->ts) = 0;
	( sm->te) = 0;
	( sm->act) = 0;
	}

#line 1501 "ext/dtext/dtext.rl"
  
#line 708 "ext/dtext/dtext.c"
	{
	if ( ( sm->p) == ( sm->pe) )
		goto _test_eof;
_resume:
	switch ( _dtext_from_state_actions[ sm->cs] ) {
	case 89:
#line 1 "NONE"
	{( sm->ts) = ( sm->p);}
	break;
#line 718 "ext/dtext/dtext.c"
	}

	switch (  sm->cs ) {
case 532:
	switch( (*( sm->p)) ) {
		case 10: goto tr620;
		case 13: goto tr621;
		case 42: goto tr622;
		case 72: goto tr623;
		case 91: goto tr624;
		case 92: goto tr625;
		case 96: goto tr626;
		case 104: goto tr623;
	}
	goto tr619;
case 533:
	switch( (*( sm->p)) ) {
		case 10: goto tr1;
		case 13: goto tr627;
	}
	goto tr0;
case 0:
	if ( (*( sm->p)) == 10 )
		goto tr1;
	goto tr0;
case 534:
	if ( (*( sm->p)) == 10 )
		goto tr620;
	goto tr628;
case 535:
	switch( (*( sm->p)) ) {
		case 9: goto tr5;
		case 32: goto tr5;
		case 42: goto tr6;
	}
	goto tr628;
case 1:
	switch( (*( sm->p)) ) {
		case 9: goto tr4;
		case 10: goto tr2;
		case 13: goto tr2;
		case 32: goto tr4;
	}
	goto tr3;
case 536:
	switch( (*( sm->p)) ) {
		case 10: goto tr629;
		case 13: goto tr629;
	}
	goto tr630;
case 537:
	switch( (*( sm->p)) ) {
		case 9: goto tr4;
		case 10: goto tr629;
		case 13: goto tr629;
		case 32: goto tr4;
	}
	goto tr3;
case 2:
	switch( (*( sm->p)) ) {
		case 9: goto tr5;
		case 32: goto tr5;
		case 42: goto tr6;
	}
	goto tr2;
case 538:
	if ( 49 <= (*( sm->p)) && (*( sm->p)) <= 54 )
		goto tr631;
	goto tr628;
case 3:
	switch( (*( sm->p)) ) {
		case 35: goto tr7;
		case 46: goto tr8;
	}
	goto tr2;
case 4:
	if ( (*( sm->p)) == 33 )
		goto tr9;
	if ( (*( sm->p)) > 45 ) {
		if ( 47 <= (*( sm->p)) && (*( sm->p)) <= 126 )
			goto tr9;
	} else if ( (*( sm->p)) >= 35 )
		goto tr9;
	goto tr2;
case 5:
	switch( (*( sm->p)) ) {
		case 33: goto tr10;
		case 46: goto tr11;
	}
	if ( 35 <= (*( sm->p)) && (*( sm->p)) <= 126 )
		goto tr10;
	goto tr2;
case 539:
	switch( (*( sm->p)) ) {
		case 9: goto tr633;
		case 32: goto tr633;
	}
	goto tr632;
case 540:
	switch( (*( sm->p)) ) {
		case 9: goto tr635;
		case 32: goto tr635;
	}
	goto tr634;
case 541:
	switch( (*( sm->p)) ) {
		case 47: goto tr636;
		case 67: goto tr637;
		case 78: goto tr638;
		case 81: goto tr639;
		case 83: goto tr640;
		case 84: goto tr641;
		case 99: goto tr637;
		case 110: goto tr638;
		case 113: goto tr639;
		case 115: goto tr640;
		case 116: goto tr641;
	}
	goto tr628;
case 6:
	switch( (*( sm->p)) ) {
		case 83: goto tr12;
		case 115: goto tr12;
	}
	goto tr2;
case 7:
	switch( (*( sm->p)) ) {
		case 80: goto tr13;
		case 112: goto tr13;
	}
	goto tr2;
case 8:
	switch( (*( sm->p)) ) {
		case 79: goto tr14;
		case 111: goto tr14;
	}
	goto tr2;
case 9:
	switch( (*( sm->p)) ) {
		case 73: goto tr15;
		case 105: goto tr15;
	}
	goto tr2;
case 10:
	switch( (*( sm->p)) ) {
		case 76: goto tr16;
		case 108: goto tr16;
	}
	goto tr2;
case 11:
	switch( (*( sm->p)) ) {
		case 69: goto tr17;
		case 101: goto tr17;
	}
	goto tr2;
case 12:
	switch( (*( sm->p)) ) {
		case 82: goto tr18;
		case 114: goto tr18;
	}
	goto tr2;
case 13:
	switch( (*( sm->p)) ) {
		case 83: goto tr19;
		case 93: goto tr20;
		case 115: goto tr19;
	}
	goto tr2;
case 14:
	if ( (*( sm->p)) == 93 )
		goto tr20;
	goto tr2;
case 15:
	switch( (*( sm->p)) ) {
		case 79: goto tr21;
		case 111: goto tr21;
	}
	goto tr2;
case 16:
	switch( (*( sm->p)) ) {
		case 68: goto tr22;
		case 100: goto tr22;
	}
	goto tr2;
case 17:
	switch( (*( sm->p)) ) {
		case 69: goto tr23;
		case 101: goto tr23;
	}
	goto tr2;
case 18:
	if ( (*( sm->p)) == 93 )
		goto tr24;
	goto tr2;
case 542:
	if ( (*( sm->p)) == 32 )
		goto tr24;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto tr24;
	goto tr642;
case 19:
	switch( (*( sm->p)) ) {
		case 79: goto tr25;
		case 111: goto tr25;
	}
	goto tr2;
case 20:
	switch( (*( sm->p)) ) {
		case 68: goto tr26;
		case 100: goto tr26;
	}
	goto tr2;
case 21:
	switch( (*( sm->p)) ) {
		case 84: goto tr27;
		case 116: goto tr27;
	}
	goto tr2;
case 22:
	switch( (*( sm->p)) ) {
		case 69: goto tr28;
		case 101: goto tr28;
	}
	goto tr2;
case 23:
	switch( (*( sm->p)) ) {
		case 88: goto tr29;
		case 120: goto tr29;
	}
	goto tr2;
case 24:
	switch( (*( sm->p)) ) {
		case 84: goto tr30;
		case 116: goto tr30;
	}
	goto tr2;
case 25:
	if ( (*( sm->p)) == 93 )
		goto tr31;
	goto tr2;
case 543:
	if ( (*( sm->p)) == 32 )
		goto tr31;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto tr31;
	goto tr643;
case 26:
	switch( (*( sm->p)) ) {
		case 85: goto tr32;
		case 117: goto tr32;
	}
	goto tr2;
case 27:
	switch( (*( sm->p)) ) {
		case 79: goto tr33;
		case 111: goto tr33;
	}
	goto tr2;
case 28:
	switch( (*( sm->p)) ) {
		case 84: goto tr34;
		case 116: goto tr34;
	}
	goto tr2;
case 29:
	switch( (*( sm->p)) ) {
		case 69: goto tr35;
		case 101: goto tr35;
	}
	goto tr2;
case 30:
	if ( (*( sm->p)) == 93 )
		goto tr36;
	goto tr2;
case 544:
	if ( (*( sm->p)) == 32 )
		goto tr36;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto tr36;
	goto tr644;
case 31:
	switch( (*( sm->p)) ) {
		case 69: goto tr37;
		case 80: goto tr38;
		case 101: goto tr37;
		case 112: goto tr38;
	}
	goto tr2;
case 32:
	switch( (*( sm->p)) ) {
		case 67: goto tr39;
		case 99: goto tr39;
	}
	goto tr2;
case 33:
	switch( (*( sm->p)) ) {
		case 84: goto tr40;
		case 116: goto tr40;
	}
	goto tr2;
case 34:
	switch( (*( sm->p)) ) {
		case 73: goto tr41;
		case 105: goto tr41;
	}
	goto tr2;
case 35:
	switch( (*( sm->p)) ) {
		case 79: goto tr42;
		case 111: goto tr42;
	}
	goto tr2;
case 36:
	switch( (*( sm->p)) ) {
		case 78: goto tr43;
		case 110: goto tr43;
	}
	goto tr2;
case 37:
	switch( (*( sm->p)) ) {
		case 44: goto tr44;
		case 61: goto tr45;
		case 93: goto tr46;
	}
	goto tr2;
case 38:
	switch( (*( sm->p)) ) {
		case 69: goto tr47;
		case 101: goto tr47;
	}
	goto tr2;
case 39:
	switch( (*( sm->p)) ) {
		case 88: goto tr48;
		case 120: goto tr48;
	}
	goto tr2;
case 40:
	switch( (*( sm->p)) ) {
		case 80: goto tr49;
		case 112: goto tr49;
	}
	goto tr2;
case 41:
	switch( (*( sm->p)) ) {
		case 65: goto tr50;
		case 97: goto tr50;
	}
	goto tr2;
case 42:
	switch( (*( sm->p)) ) {
		case 78: goto tr51;
		case 110: goto tr51;
	}
	goto tr2;
case 43:
	switch( (*( sm->p)) ) {
		case 68: goto tr52;
		case 100: goto tr52;
	}
	goto tr2;
case 44:
	switch( (*( sm->p)) ) {
		case 69: goto tr53;
		case 101: goto tr53;
	}
	goto tr2;
case 45:
	switch( (*( sm->p)) ) {
		case 68: goto tr54;
		case 100: goto tr54;
	}
	goto tr2;
case 46:
	switch( (*( sm->p)) ) {
		case 61: goto tr55;
		case 93: goto tr56;
	}
	goto tr2;
case 47:
	if ( (*( sm->p)) == 93 )
		goto tr2;
	goto tr57;
case 48:
	if ( (*( sm->p)) == 93 )
		goto tr59;
	goto tr58;
case 545:
	if ( (*( sm->p)) == 32 )
		goto tr646;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto tr646;
	goto tr645;
case 546:
	if ( (*( sm->p)) == 32 )
		goto tr56;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto tr56;
	goto tr647;
case 49:
	if ( (*( sm->p)) == 93 )
		goto tr2;
	goto tr60;
case 50:
	if ( (*( sm->p)) == 93 )
		goto tr62;
	goto tr61;
case 547:
	if ( (*( sm->p)) == 32 )
		goto tr649;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto tr649;
	goto tr648;
case 548:
	if ( (*( sm->p)) == 32 )
		goto tr46;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto tr46;
	goto tr650;
case 51:
	switch( (*( sm->p)) ) {
		case 79: goto tr63;
		case 111: goto tr63;
	}
	goto tr2;
case 52:
	switch( (*( sm->p)) ) {
		case 73: goto tr64;
		case 105: goto tr64;
	}
	goto tr2;
case 53:
	switch( (*( sm->p)) ) {
		case 76: goto tr65;
		case 108: goto tr65;
	}
	goto tr2;
case 54:
	switch( (*( sm->p)) ) {
		case 69: goto tr66;
		case 101: goto tr66;
	}
	goto tr2;
case 55:
	switch( (*( sm->p)) ) {
		case 82: goto tr67;
		case 114: goto tr67;
	}
	goto tr2;
case 56:
	switch( (*( sm->p)) ) {
		case 83: goto tr68;
		case 93: goto tr69;
		case 115: goto tr68;
	}
	goto tr2;
case 57:
	if ( (*( sm->p)) == 93 )
		goto tr69;
	goto tr2;
case 549:
	if ( (*( sm->p)) == 32 )
		goto tr69;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto tr69;
	goto tr651;
case 58:
	switch( (*( sm->p)) ) {
		case 65: goto tr70;
		case 78: goto tr71;
		case 97: goto tr70;
		case 110: goto tr71;
	}
	goto tr2;
case 59:
	switch( (*( sm->p)) ) {
		case 66: goto tr72;
		case 98: goto tr72;
	}
	goto tr2;
case 60:
	switch( (*( sm->p)) ) {
		case 76: goto tr73;
		case 108: goto tr73;
	}
	goto tr2;
case 61:
	switch( (*( sm->p)) ) {
		case 69: goto tr74;
		case 101: goto tr74;
	}
	goto tr2;
case 62:
	if ( (*( sm->p)) == 93 )
		goto tr75;
	goto tr2;
case 63:
	if ( (*( sm->p)) == 93 )
		goto tr76;
	goto tr2;
case 550:
	if ( (*( sm->p)) == 96 )
		goto tr652;
	goto tr628;
case 551:
	if ( (*( sm->p)) == 91 )
		goto tr654;
	goto tr653;
case 552:
	switch( (*( sm->p)) ) {
		case 47: goto tr656;
		case 66: goto tr657;
		case 73: goto tr658;
		case 83: goto tr659;
		case 85: goto tr660;
		case 98: goto tr657;
		case 105: goto tr658;
		case 115: goto tr659;
		case 117: goto tr660;
	}
	goto tr655;
case 64:
	switch( (*( sm->p)) ) {
		case 66: goto tr78;
		case 73: goto tr79;
		case 83: goto tr80;
		case 85: goto tr81;
		case 98: goto tr78;
		case 105: goto tr79;
		case 115: goto tr80;
		case 117: goto tr81;
	}
	goto tr77;
case 65:
	if ( (*( sm->p)) == 93 )
		goto tr82;
	goto tr77;
case 66:
	if ( (*( sm->p)) == 93 )
		goto tr83;
	goto tr77;
case 67:
	switch( (*( sm->p)) ) {
		case 85: goto tr84;
		case 93: goto tr85;
		case 117: goto tr84;
	}
	goto tr77;
case 68:
	switch( (*( sm->p)) ) {
		case 66: goto tr86;
		case 80: goto tr87;
		case 98: goto tr86;
		case 112: goto tr87;
	}
	goto tr77;
case 69:
	if ( (*( sm->p)) == 93 )
		goto tr88;
	goto tr77;
case 70:
	if ( (*( sm->p)) == 93 )
		goto tr89;
	goto tr77;
case 71:
	if ( (*( sm->p)) == 93 )
		goto tr90;
	goto tr77;
case 72:
	if ( (*( sm->p)) == 93 )
		goto tr91;
	goto tr77;
case 73:
	if ( (*( sm->p)) == 93 )
		goto tr92;
	goto tr77;
case 74:
	switch( (*( sm->p)) ) {
		case 85: goto tr93;
		case 93: goto tr94;
		case 117: goto tr93;
	}
	goto tr77;
case 75:
	switch( (*( sm->p)) ) {
		case 66: goto tr95;
		case 80: goto tr96;
		case 98: goto tr95;
		case 112: goto tr96;
	}
	goto tr77;
case 76:
	if ( (*( sm->p)) == 93 )
		goto tr97;
	goto tr77;
case 77:
	if ( (*( sm->p)) == 93 )
		goto tr98;
	goto tr77;
case 78:
	if ( (*( sm->p)) == 93 )
		goto tr99;
	goto tr77;
case 553:
	switch( (*( sm->p)) ) {
		case 10: goto tr662;
		case 13: goto tr663;
		case 34: goto tr664;
		case 60: goto tr665;
		case 64: goto tr666;
		case 65: goto tr667;
		case 66: goto tr668;
		case 67: goto tr669;
		case 70: goto tr670;
		case 73: goto tr671;
		case 77: goto tr672;
		case 78: goto tr673;
		case 80: goto tr674;
		case 82: goto tr675;
		case 83: goto tr676;
		case 84: goto tr677;
		case 85: goto tr678;
		case 87: goto tr679;
		case 91: goto tr680;
		case 92: goto tr681;
		case 96: goto tr682;
		case 97: goto tr667;
		case 98: goto tr668;
		case 99: goto tr669;
		case 102: goto tr670;
		case 104: goto tr683;
		case 105: goto tr671;
		case 109: goto tr672;
		case 110: goto tr673;
		case 112: goto tr674;
		case 114: goto tr675;
		case 115: goto tr676;
		case 116: goto tr677;
		case 117: goto tr678;
		case 119: goto tr679;
		case 123: goto tr684;
	}
	goto tr661;
case 554:
	switch( (*( sm->p)) ) {
		case 10: goto tr101;
		case 13: goto tr686;
		case 42: goto tr687;
	}
	goto tr685;
case 555:
	switch( (*( sm->p)) ) {
		case 10: goto tr101;
		case 13: goto tr686;
	}
	goto tr688;
case 79:
	if ( (*( sm->p)) == 10 )
		goto tr101;
	goto tr100;
case 80:
	switch( (*( sm->p)) ) {
		case 9: goto tr103;
		case 32: goto tr103;
		case 42: goto tr104;
	}
	goto tr102;
case 81:
	switch( (*( sm->p)) ) {
		case 9: goto tr106;
		case 10: goto tr102;
		case 13: goto tr102;
		case 32: goto tr106;
	}
	goto tr105;
case 556:
	switch( (*( sm->p)) ) {
		case 10: goto tr689;
		case 13: goto tr689;
	}
	goto tr690;
case 557:
	switch( (*( sm->p)) ) {
		case 9: goto tr106;
		case 10: goto tr689;
		case 13: goto tr689;
		case 32: goto tr106;
	}
	goto tr105;
case 558:
	if ( (*( sm->p)) == 10 )
		goto tr662;
	goto tr691;
case 559:
	if ( (*( sm->p)) == 34 )
		goto tr692;
	goto tr693;
case 82:
	if ( (*( sm->p)) == 34 )
		goto tr109;
	goto tr108;
case 83:
	if ( (*( sm->p)) == 58 )
		goto tr110;
	goto tr107;
case 84:
	switch( (*( sm->p)) ) {
		case 35: goto tr111;
		case 47: goto tr111;
		case 91: goto tr112;
		case 104: goto tr113;
	}
	goto tr107;
case 85:
	if ( (*( sm->p)) < -32 ) {
		if ( -62 <= (*( sm->p)) && (*( sm->p)) <= -33 )
			goto tr114;
	} else if ( (*( sm->p)) > -17 ) {
		if ( (*( sm->p)) > -12 ) {
			if ( 33 <= (*( sm->p)) && (*( sm->p)) <= 126 )
				goto tr117;
		} else if ( (*( sm->p)) >= -16 )
			goto tr116;
	} else
		goto tr115;
	goto tr107;
case 86:
	if ( (*( sm->p)) <= -65 )
		goto tr117;
	goto tr100;
case 560:
	if ( (*( sm->p)) < -32 ) {
		if ( -62 <= (*( sm->p)) && (*( sm->p)) <= -33 )
			goto tr114;
	} else if ( (*( sm->p)) > -17 ) {
		if ( (*( sm->p)) > -12 ) {
			if ( 33 <= (*( sm->p)) && (*( sm->p)) <= 126 )
				goto tr117;
		} else if ( (*( sm->p)) >= -16 )
			goto tr116;
	} else
		goto tr115;
	goto tr694;
case 87:
	if ( (*( sm->p)) <= -65 )
		goto tr114;
	goto tr100;
case 88:
	if ( (*( sm->p)) <= -65 )
		goto tr115;
	goto tr100;
case 89:
	switch( (*( sm->p)) ) {
		case 35: goto tr118;
		case 47: goto tr118;
		case 104: goto tr119;
	}
	goto tr107;
case 90:
	if ( (*( sm->p)) < -32 ) {
		if ( -62 <= (*( sm->p)) && (*( sm->p)) <= -33 )
			goto tr120;
	} else if ( (*( sm->p)) > -17 ) {
		if ( (*( sm->p)) > -12 ) {
			if ( 33 <= (*( sm->p)) && (*( sm->p)) <= 126 )
				goto tr123;
		} else if ( (*( sm->p)) >= -16 )
			goto tr122;
	} else
		goto tr121;
	goto tr107;
case 91:
	if ( (*( sm->p)) <= -65 )
		goto tr123;
	goto tr107;
case 92:
	if ( (*( sm->p)) == 93 )
		goto tr124;
	if ( (*( sm->p)) < -32 ) {
		if ( -62 <= (*( sm->p)) && (*( sm->p)) <= -33 )
			goto tr120;
	} else if ( (*( sm->p)) > -17 ) {
		if ( (*( sm->p)) > -12 ) {
			if ( 33 <= (*( sm->p)) && (*( sm->p)) <= 126 )
				goto tr123;
		} else if ( (*( sm->p)) >= -16 )
			goto tr122;
	} else
		goto tr121;
	goto tr107;
case 93:
	if ( (*( sm->p)) <= -65 )
		goto tr120;
	goto tr107;
case 94:
	if ( (*( sm->p)) <= -65 )
		goto tr121;
	goto tr107;
case 95:
	if ( (*( sm->p)) == 116 )
		goto tr125;
	goto tr107;
case 96:
	if ( (*( sm->p)) == 116 )
		goto tr126;
	goto tr107;
case 97:
	if ( (*( sm->p)) == 112 )
		goto tr127;
	goto tr107;
case 98:
	switch( (*( sm->p)) ) {
		case 58: goto tr128;
		case 115: goto tr129;
	}
	goto tr107;
case 99:
	if ( (*( sm->p)) == 47 )
		goto tr130;
	goto tr107;
case 100:
	if ( (*( sm->p)) == 47 )
		goto tr131;
	goto tr107;
case 101:
	if ( (*( sm->p)) == 58 )
		goto tr128;
	goto tr107;
case 102:
	if ( (*( sm->p)) == 116 )
		goto tr132;
	goto tr107;
case 103:
	if ( (*( sm->p)) == 116 )
		goto tr133;
	goto tr107;
case 104:
	if ( (*( sm->p)) == 112 )
		goto tr134;
	goto tr107;
case 105:
	switch( (*( sm->p)) ) {
		case 58: goto tr135;
		case 115: goto tr136;
	}
	goto tr107;
case 106:
	if ( (*( sm->p)) == 47 )
		goto tr137;
	goto tr107;
case 107:
	if ( (*( sm->p)) == 47 )
		goto tr138;
	goto tr107;
case 108:
	if ( (*( sm->p)) == 58 )
		goto tr135;
	goto tr107;
case 561:
	switch( (*( sm->p)) ) {
		case 64: goto tr695;
		case 104: goto tr696;
	}
	goto tr692;
case 109:
	if ( (*( sm->p)) < -32 ) {
		if ( -62 <= (*( sm->p)) && (*( sm->p)) <= -33 )
			goto tr139;
	} else if ( (*( sm->p)) > -17 ) {
		if ( (*( sm->p)) > -12 ) {
			if ( 33 <= (*( sm->p)) && (*( sm->p)) <= 126 )
				goto tr142;
		} else if ( (*( sm->p)) >= -16 )
			goto tr141;
	} else
		goto tr140;
	goto tr107;
case 110:
	if ( (*( sm->p)) <= -65 )
		goto tr143;
	goto tr107;
case 111:
	if ( (*( sm->p)) == 62 )
		goto tr147;
	if ( (*( sm->p)) < -32 ) {
		if ( -62 <= (*( sm->p)) && (*( sm->p)) <= -33 )
			goto tr144;
	} else if ( (*( sm->p)) > -17 ) {
		if ( (*( sm->p)) > -12 ) {
			if ( 33 <= (*( sm->p)) && (*( sm->p)) <= 126 )
				goto tr143;
		} else if ( (*( sm->p)) >= -16 )
			goto tr146;
	} else
		goto tr145;
	goto tr107;
case 112:
	if ( (*( sm->p)) <= -65 )
		goto tr144;
	goto tr107;
case 113:
	if ( (*( sm->p)) <= -65 )
		goto tr145;
	goto tr107;
case 114:
	if ( (*( sm->p)) == 116 )
		goto tr148;
	goto tr107;
case 115:
	if ( (*( sm->p)) == 116 )
		goto tr149;
	goto tr107;
case 116:
	if ( (*( sm->p)) == 112 )
		goto tr150;
	goto tr107;
case 117:
	switch( (*( sm->p)) ) {
		case 58: goto tr151;
		case 115: goto tr152;
	}
	goto tr107;
case 118:
	if ( (*( sm->p)) == 47 )
		goto tr153;
	goto tr107;
case 119:
	if ( (*( sm->p)) == 47 )
		goto tr154;
	goto tr107;
case 120:
	if ( (*( sm->p)) < -32 ) {
		if ( -62 <= (*( sm->p)) && (*( sm->p)) <= -33 )
			goto tr155;
	} else if ( (*( sm->p)) > -17 ) {
		if ( (*( sm->p)) > -12 ) {
			if ( 33 <= (*( sm->p)) && (*( sm->p)) <= 126 )
				goto tr158;
		} else if ( (*( sm->p)) >= -16 )
			goto tr157;
	} else
		goto tr156;
	goto tr107;
case 121:
	if ( (*( sm->p)) <= -65 )
		goto tr158;
	goto tr107;
case 122:
	if ( (*( sm->p)) == 62 )
		goto tr159;
	if ( (*( sm->p)) < -32 ) {
		if ( -62 <= (*( sm->p)) && (*( sm->p)) <= -33 )
			goto tr155;
	} else if ( (*( sm->p)) > -17 ) {
		if ( (*( sm->p)) > -12 ) {
			if ( 33 <= (*( sm->p)) && (*( sm->p)) <= 126 )
				goto tr158;
		} else if ( (*( sm->p)) >= -16 )
			goto tr157;
	} else
		goto tr156;
	goto tr107;
case 123:
	if ( (*( sm->p)) <= -65 )
		goto tr155;
	goto tr107;
case 124:
	if ( (*( sm->p)) <= -65 )
		goto tr156;
	goto tr107;
case 125:
	if ( (*( sm->p)) == 58 )
		goto tr151;
	goto tr107;
case 562:
	if ( (*( sm->p)) < -32 ) {
		if ( -62 <= (*( sm->p)) && (*( sm->p)) <= -33 )
			goto tr697;
	} else if ( (*( sm->p)) > -17 ) {
		if ( (*( sm->p)) > -12 ) {
			if ( 33 <= (*( sm->p)) && (*( sm->p)) <= 126 )
				goto tr700;
		} else if ( (*( sm->p)) >= -16 )
			goto tr699;
	} else
		goto tr698;
	goto tr692;
case 126:
	if ( (*( sm->p)) <= -65 )
		goto tr160;
	goto tr100;
case 563:
	if ( (*( sm->p)) < -32 ) {
		if ( -62 <= (*( sm->p)) && (*( sm->p)) <= -33 )
			goto tr161;
	} else if ( (*( sm->p)) > -17 ) {
		if ( (*( sm->p)) > -12 ) {
			if ( 33 <= (*( sm->p)) && (*( sm->p)) <= 126 )
				goto tr160;
		} else if ( (*( sm->p)) >= -16 )
			goto tr702;
	} else
		goto tr162;
	goto tr701;
case 127:
	if ( (*( sm->p)) <= -65 )
		goto tr161;
	goto tr100;
case 128:
	if ( (*( sm->p)) <= -65 )
		goto tr162;
	goto tr100;
case 564:
	if ( (*( sm->p)) == 64 )
		goto tr704;
	if ( (*( sm->p)) < -32 ) {
		if ( -62 <= (*( sm->p)) && (*( sm->p)) <= -33 )
			goto tr161;
	} else if ( (*( sm->p)) > -17 ) {
		if ( (*( sm->p)) > -12 ) {
			if ( 33 <= (*( sm->p)) && (*( sm->p)) <= 126 )
				goto tr160;
		} else if ( (*( sm->p)) >= -16 )
			goto tr702;
	} else
		goto tr162;
	goto tr703;
case 565:
	switch( (*( sm->p)) ) {
		case 76: goto tr705;
		case 82: goto tr706;
		case 108: goto tr705;
		case 114: goto tr706;
	}
	goto tr692;
case 129:
	switch( (*( sm->p)) ) {
		case 73: goto tr163;
		case 105: goto tr163;
	}
	goto tr107;
case 130:
	switch( (*( sm->p)) ) {
		case 65: goto tr164;
		case 97: goto tr164;
	}
	goto tr107;
case 131:
	switch( (*( sm->p)) ) {
		case 83: goto tr165;
		case 115: goto tr165;
	}
	goto tr107;
case 132:
	if ( (*( sm->p)) == 32 )
		goto tr166;
	goto tr107;
case 133:
	if ( (*( sm->p)) == 35 )
		goto tr167;
	goto tr107;
case 134:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr168;
	goto tr107;
case 566:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr708;
	goto tr707;
case 135:
	switch( (*( sm->p)) ) {
		case 84: goto tr169;
		case 116: goto tr169;
	}
	goto tr107;
case 136:
	switch( (*( sm->p)) ) {
		case 73: goto tr170;
		case 105: goto tr170;
	}
	goto tr107;
case 137:
	switch( (*( sm->p)) ) {
		case 83: goto tr171;
		case 115: goto tr171;
	}
	goto tr107;
case 138:
	switch( (*( sm->p)) ) {
		case 84: goto tr172;
		case 116: goto tr172;
	}
	goto tr107;
case 139:
	if ( (*( sm->p)) == 32 )
		goto tr173;
	goto tr107;
case 140:
	if ( (*( sm->p)) == 35 )
		goto tr174;
	goto tr107;
case 141:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr175;
	goto tr107;
case 567:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr710;
	goto tr709;
case 568:
	switch( (*( sm->p)) ) {
		case 65: goto tr711;
		case 76: goto tr712;
		case 85: goto tr713;
		case 97: goto tr711;
		case 108: goto tr712;
		case 117: goto tr713;
	}
	goto tr692;
case 142:
	switch( (*( sm->p)) ) {
		case 78: goto tr176;
		case 110: goto tr176;
	}
	goto tr107;
case 143:
	if ( (*( sm->p)) == 32 )
		goto tr177;
	goto tr107;
case 144:
	if ( (*( sm->p)) == 35 )
		goto tr178;
	goto tr107;
case 145:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr179;
	goto tr107;
case 569:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr715;
	goto tr714;
case 146:
	switch( (*( sm->p)) ) {
		case 73: goto tr180;
		case 105: goto tr180;
	}
	goto tr107;
case 147:
	switch( (*( sm->p)) ) {
		case 80: goto tr181;
		case 112: goto tr181;
	}
	goto tr107;
case 148:
	if ( (*( sm->p)) == 32 )
		goto tr182;
	goto tr107;
case 149:
	if ( (*( sm->p)) == 35 )
		goto tr183;
	goto tr107;
case 150:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr184;
	goto tr107;
case 570:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr717;
	goto tr716;
case 151:
	switch( (*( sm->p)) ) {
		case 82: goto tr185;
		case 114: goto tr185;
	}
	goto tr107;
case 152:
	if ( (*( sm->p)) == 32 )
		goto tr186;
	goto tr107;
case 153:
	if ( (*( sm->p)) == 35 )
		goto tr187;
	goto tr107;
case 154:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr188;
	goto tr107;
case 571:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr719;
	goto tr718;
case 572:
	switch( (*( sm->p)) ) {
		case 79: goto tr720;
		case 111: goto tr720;
	}
	goto tr692;
case 155:
	switch( (*( sm->p)) ) {
		case 77: goto tr189;
		case 109: goto tr189;
	}
	goto tr107;
case 156:
	switch( (*( sm->p)) ) {
		case 77: goto tr190;
		case 109: goto tr190;
	}
	goto tr107;
case 157:
	switch( (*( sm->p)) ) {
		case 69: goto tr191;
		case 101: goto tr191;
	}
	goto tr107;
case 158:
	switch( (*( sm->p)) ) {
		case 78: goto tr192;
		case 110: goto tr192;
	}
	goto tr107;
case 159:
	switch( (*( sm->p)) ) {
		case 84: goto tr193;
		case 116: goto tr193;
	}
	goto tr107;
case 160:
	if ( (*( sm->p)) == 32 )
		goto tr194;
	goto tr107;
case 161:
	if ( (*( sm->p)) == 35 )
		goto tr195;
	goto tr107;
case 162:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr196;
	goto tr107;
case 573:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr722;
	goto tr721;
case 574:
	switch( (*( sm->p)) ) {
		case 76: goto tr723;
		case 79: goto tr724;
		case 108: goto tr723;
		case 111: goto tr724;
	}
	goto tr692;
case 163:
	switch( (*( sm->p)) ) {
		case 65: goto tr197;
		case 97: goto tr197;
	}
	goto tr107;
case 164:
	switch( (*( sm->p)) ) {
		case 71: goto tr198;
		case 103: goto tr198;
	}
	goto tr107;
case 165:
	if ( (*( sm->p)) == 32 )
		goto tr199;
	goto tr107;
case 166:
	if ( (*( sm->p)) == 35 )
		goto tr200;
	goto tr107;
case 167:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr201;
	goto tr107;
case 575:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr726;
	goto tr725;
case 168:
	switch( (*( sm->p)) ) {
		case 82: goto tr202;
		case 114: goto tr202;
	}
	goto tr107;
case 169:
	switch( (*( sm->p)) ) {
		case 85: goto tr203;
		case 117: goto tr203;
	}
	goto tr107;
case 170:
	switch( (*( sm->p)) ) {
		case 77: goto tr204;
		case 109: goto tr204;
	}
	goto tr107;
case 171:
	if ( (*( sm->p)) == 32 )
		goto tr205;
	goto tr107;
case 172:
	if ( (*( sm->p)) == 35 )
		goto tr206;
	goto tr107;
case 173:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr207;
	goto tr107;
case 576:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr728;
	goto tr727;
case 577:
	switch( (*( sm->p)) ) {
		case 77: goto tr729;
		case 109: goto tr729;
	}
	goto tr692;
case 174:
	switch( (*( sm->p)) ) {
		case 80: goto tr208;
		case 112: goto tr208;
	}
	goto tr107;
case 175:
	switch( (*( sm->p)) ) {
		case 76: goto tr209;
		case 108: goto tr209;
	}
	goto tr107;
case 176:
	switch( (*( sm->p)) ) {
		case 73: goto tr210;
		case 105: goto tr210;
	}
	goto tr107;
case 177:
	switch( (*( sm->p)) ) {
		case 67: goto tr211;
		case 99: goto tr211;
	}
	goto tr107;
case 178:
	switch( (*( sm->p)) ) {
		case 65: goto tr212;
		case 97: goto tr212;
	}
	goto tr107;
case 179:
	switch( (*( sm->p)) ) {
		case 84: goto tr213;
		case 116: goto tr213;
	}
	goto tr107;
case 180:
	switch( (*( sm->p)) ) {
		case 73: goto tr214;
		case 105: goto tr214;
	}
	goto tr107;
case 181:
	switch( (*( sm->p)) ) {
		case 79: goto tr215;
		case 111: goto tr215;
	}
	goto tr107;
case 182:
	switch( (*( sm->p)) ) {
		case 78: goto tr216;
		case 110: goto tr216;
	}
	goto tr107;
case 183:
	if ( (*( sm->p)) == 32 )
		goto tr217;
	goto tr107;
case 184:
	if ( (*( sm->p)) == 35 )
		goto tr218;
	goto tr107;
case 185:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr219;
	goto tr107;
case 578:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr731;
	goto tr730;
case 579:
	switch( (*( sm->p)) ) {
		case 79: goto tr732;
		case 111: goto tr732;
	}
	goto tr692;
case 186:
	switch( (*( sm->p)) ) {
		case 68: goto tr220;
		case 100: goto tr220;
	}
	goto tr107;
case 187:
	if ( (*( sm->p)) == 32 )
		goto tr221;
	goto tr107;
case 188:
	switch( (*( sm->p)) ) {
		case 65: goto tr222;
		case 97: goto tr222;
	}
	goto tr107;
case 189:
	switch( (*( sm->p)) ) {
		case 67: goto tr223;
		case 99: goto tr223;
	}
	goto tr107;
case 190:
	switch( (*( sm->p)) ) {
		case 84: goto tr224;
		case 116: goto tr224;
	}
	goto tr107;
case 191:
	switch( (*( sm->p)) ) {
		case 73: goto tr225;
		case 105: goto tr225;
	}
	goto tr107;
case 192:
	switch( (*( sm->p)) ) {
		case 79: goto tr226;
		case 111: goto tr226;
	}
	goto tr107;
case 193:
	switch( (*( sm->p)) ) {
		case 78: goto tr227;
		case 110: goto tr227;
	}
	goto tr107;
case 194:
	if ( (*( sm->p)) == 32 )
		goto tr228;
	goto tr107;
case 195:
	if ( (*( sm->p)) == 35 )
		goto tr229;
	goto tr107;
case 196:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr230;
	goto tr107;
case 580:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr734;
	goto tr733;
case 581:
	switch( (*( sm->p)) ) {
		case 79: goto tr735;
		case 111: goto tr735;
	}
	goto tr692;
case 197:
	switch( (*( sm->p)) ) {
		case 84: goto tr231;
		case 116: goto tr231;
	}
	goto tr107;
case 198:
	switch( (*( sm->p)) ) {
		case 69: goto tr232;
		case 101: goto tr232;
	}
	goto tr107;
case 199:
	if ( (*( sm->p)) == 32 )
		goto tr233;
	goto tr107;
case 200:
	if ( (*( sm->p)) == 35 )
		goto tr234;
	goto tr107;
case 201:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr235;
	goto tr107;
case 582:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr737;
	goto tr736;
case 583:
	switch( (*( sm->p)) ) {
		case 79: goto tr738;
		case 111: goto tr738;
	}
	goto tr692;
case 202:
	switch( (*( sm->p)) ) {
		case 79: goto tr236;
		case 83: goto tr237;
		case 111: goto tr236;
		case 115: goto tr237;
	}
	goto tr107;
case 203:
	switch( (*( sm->p)) ) {
		case 76: goto tr238;
		case 108: goto tr238;
	}
	goto tr107;
case 204:
	if ( (*( sm->p)) == 32 )
		goto tr239;
	goto tr107;
case 205:
	if ( (*( sm->p)) == 35 )
		goto tr240;
	goto tr107;
case 206:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr241;
	goto tr107;
case 584:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr740;
	goto tr739;
case 207:
	switch( (*( sm->p)) ) {
		case 84: goto tr242;
		case 116: goto tr242;
	}
	goto tr107;
case 208:
	if ( (*( sm->p)) == 32 )
		goto tr243;
	goto tr107;
case 209:
	switch( (*( sm->p)) ) {
		case 35: goto tr244;
		case 67: goto tr245;
		case 99: goto tr245;
	}
	goto tr107;
case 210:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr246;
	goto tr107;
case 585:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr742;
	goto tr741;
case 211:
	switch( (*( sm->p)) ) {
		case 72: goto tr247;
		case 104: goto tr247;
	}
	goto tr107;
case 212:
	switch( (*( sm->p)) ) {
		case 65: goto tr248;
		case 97: goto tr248;
	}
	goto tr107;
case 213:
	switch( (*( sm->p)) ) {
		case 78: goto tr249;
		case 110: goto tr249;
	}
	goto tr107;
case 214:
	switch( (*( sm->p)) ) {
		case 71: goto tr250;
		case 103: goto tr250;
	}
	goto tr107;
case 215:
	switch( (*( sm->p)) ) {
		case 69: goto tr251;
		case 101: goto tr251;
	}
	goto tr107;
case 216:
	switch( (*( sm->p)) ) {
		case 83: goto tr252;
		case 115: goto tr252;
	}
	goto tr107;
case 217:
	if ( (*( sm->p)) == 32 )
		goto tr253;
	goto tr107;
case 218:
	if ( (*( sm->p)) == 35 )
		goto tr254;
	goto tr107;
case 219:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr255;
	goto tr107;
case 586:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr744;
	goto tr743;
case 587:
	switch( (*( sm->p)) ) {
		case 69: goto tr745;
		case 101: goto tr745;
	}
	goto tr692;
case 220:
	switch( (*( sm->p)) ) {
		case 67: goto tr256;
		case 99: goto tr256;
	}
	goto tr107;
case 221:
	switch( (*( sm->p)) ) {
		case 79: goto tr257;
		case 111: goto tr257;
	}
	goto tr107;
case 222:
	switch( (*( sm->p)) ) {
		case 82: goto tr258;
		case 114: goto tr258;
	}
	goto tr107;
case 223:
	switch( (*( sm->p)) ) {
		case 68: goto tr259;
		case 100: goto tr259;
	}
	goto tr107;
case 224:
	if ( (*( sm->p)) == 32 )
		goto tr260;
	goto tr107;
case 225:
	if ( (*( sm->p)) == 35 )
		goto tr261;
	goto tr107;
case 226:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr262;
	goto tr107;
case 588:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr747;
	goto tr746;
case 589:
	switch( (*( sm->p)) ) {
		case 69: goto tr748;
		case 101: goto tr748;
	}
	goto tr692;
case 227:
	switch( (*( sm->p)) ) {
		case 84: goto tr263;
		case 116: goto tr263;
	}
	goto tr107;
case 228:
	if ( (*( sm->p)) == 32 )
		goto tr264;
	goto tr107;
case 229:
	if ( (*( sm->p)) == 35 )
		goto tr265;
	goto tr107;
case 230:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr266;
	goto tr107;
case 590:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr750;
	goto tr749;
case 591:
	switch( (*( sm->p)) ) {
		case 65: goto tr751;
		case 72: goto tr752;
		case 73: goto tr753;
		case 79: goto tr754;
		case 97: goto tr751;
		case 104: goto tr752;
		case 105: goto tr753;
		case 111: goto tr754;
	}
	goto tr692;
case 231:
	switch( (*( sm->p)) ) {
		case 75: goto tr267;
		case 107: goto tr267;
	}
	goto tr107;
case 232:
	switch( (*( sm->p)) ) {
		case 69: goto tr268;
		case 101: goto tr268;
	}
	goto tr107;
case 233:
	switch( (*( sm->p)) ) {
		case 32: goto tr269;
		case 68: goto tr270;
		case 100: goto tr270;
	}
	goto tr107;
case 234:
	switch( (*( sm->p)) ) {
		case 68: goto tr270;
		case 100: goto tr270;
	}
	goto tr107;
case 235:
	switch( (*( sm->p)) ) {
		case 79: goto tr271;
		case 111: goto tr271;
	}
	goto tr107;
case 236:
	switch( (*( sm->p)) ) {
		case 87: goto tr272;
		case 119: goto tr272;
	}
	goto tr107;
case 237:
	switch( (*( sm->p)) ) {
		case 78: goto tr273;
		case 110: goto tr273;
	}
	goto tr107;
case 238:
	if ( (*( sm->p)) == 32 )
		goto tr274;
	goto tr107;
case 239:
	switch( (*( sm->p)) ) {
		case 35: goto tr275;
		case 82: goto tr276;
		case 114: goto tr276;
	}
	goto tr107;
case 240:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr277;
	goto tr107;
case 592:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr756;
	goto tr755;
case 241:
	switch( (*( sm->p)) ) {
		case 69: goto tr278;
		case 101: goto tr278;
	}
	goto tr107;
case 242:
	switch( (*( sm->p)) ) {
		case 81: goto tr279;
		case 113: goto tr279;
	}
	goto tr107;
case 243:
	switch( (*( sm->p)) ) {
		case 85: goto tr280;
		case 117: goto tr280;
	}
	goto tr107;
case 244:
	switch( (*( sm->p)) ) {
		case 69: goto tr281;
		case 101: goto tr281;
	}
	goto tr107;
case 245:
	switch( (*( sm->p)) ) {
		case 83: goto tr282;
		case 115: goto tr282;
	}
	goto tr107;
case 246:
	switch( (*( sm->p)) ) {
		case 84: goto tr283;
		case 116: goto tr283;
	}
	goto tr107;
case 247:
	if ( (*( sm->p)) == 32 )
		goto tr284;
	goto tr107;
case 248:
	if ( (*( sm->p)) == 35 )
		goto tr275;
	goto tr107;
case 249:
	switch( (*( sm->p)) ) {
		case 85: goto tr285;
		case 117: goto tr285;
	}
	goto tr107;
case 250:
	switch( (*( sm->p)) ) {
		case 77: goto tr286;
		case 109: goto tr286;
	}
	goto tr107;
case 251:
	switch( (*( sm->p)) ) {
		case 66: goto tr287;
		case 98: goto tr287;
	}
	goto tr107;
case 252:
	if ( (*( sm->p)) == 32 )
		goto tr288;
	goto tr107;
case 253:
	if ( (*( sm->p)) == 35 )
		goto tr289;
	goto tr107;
case 254:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr290;
	goto tr107;
case 593:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr758;
	goto tr757;
case 255:
	switch( (*( sm->p)) ) {
		case 67: goto tr291;
		case 99: goto tr291;
	}
	goto tr107;
case 256:
	switch( (*( sm->p)) ) {
		case 75: goto tr292;
		case 107: goto tr292;
	}
	goto tr107;
case 257:
	switch( (*( sm->p)) ) {
		case 69: goto tr293;
		case 101: goto tr293;
	}
	goto tr107;
case 258:
	switch( (*( sm->p)) ) {
		case 84: goto tr294;
		case 116: goto tr294;
	}
	goto tr107;
case 259:
	if ( (*( sm->p)) == 32 )
		goto tr295;
	goto tr107;
case 260:
	if ( (*( sm->p)) == 35 )
		goto tr296;
	goto tr107;
case 261:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr297;
	goto tr107;
case 594:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr760;
	goto tr759;
case 262:
	switch( (*( sm->p)) ) {
		case 80: goto tr298;
		case 112: goto tr298;
	}
	goto tr107;
case 263:
	switch( (*( sm->p)) ) {
		case 73: goto tr299;
		case 105: goto tr299;
	}
	goto tr107;
case 264:
	switch( (*( sm->p)) ) {
		case 67: goto tr300;
		case 99: goto tr300;
	}
	goto tr107;
case 265:
	if ( (*( sm->p)) == 32 )
		goto tr301;
	goto tr107;
case 266:
	if ( (*( sm->p)) == 35 )
		goto tr302;
	goto tr107;
case 267:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr303;
	goto tr107;
case 595:
	if ( (*( sm->p)) == 47 )
		goto tr762;
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr763;
	goto tr761;
case 268:
	switch( (*( sm->p)) ) {
		case 80: goto tr305;
		case 112: goto tr305;
	}
	goto tr304;
case 269:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr306;
	goto tr304;
case 596:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr765;
	goto tr764;
case 597:
	switch( (*( sm->p)) ) {
		case 83: goto tr766;
		case 115: goto tr766;
	}
	goto tr692;
case 270:
	switch( (*( sm->p)) ) {
		case 69: goto tr307;
		case 101: goto tr307;
	}
	goto tr107;
case 271:
	switch( (*( sm->p)) ) {
		case 82: goto tr308;
		case 114: goto tr308;
	}
	goto tr107;
case 272:
	if ( (*( sm->p)) == 32 )
		goto tr309;
	goto tr107;
case 273:
	if ( (*( sm->p)) == 35 )
		goto tr310;
	goto tr107;
case 274:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr311;
	goto tr107;
case 598:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr768;
	goto tr767;
case 599:
	switch( (*( sm->p)) ) {
		case 73: goto tr769;
		case 105: goto tr769;
	}
	goto tr692;
case 275:
	switch( (*( sm->p)) ) {
		case 75: goto tr312;
		case 107: goto tr312;
	}
	goto tr107;
case 276:
	switch( (*( sm->p)) ) {
		case 73: goto tr313;
		case 105: goto tr313;
	}
	goto tr107;
case 277:
	if ( (*( sm->p)) == 32 )
		goto tr314;
	goto tr107;
case 278:
	if ( (*( sm->p)) == 35 )
		goto tr315;
	goto tr107;
case 279:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr316;
	goto tr107;
case 600:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr771;
	goto tr770;
case 601:
	switch( (*( sm->p)) ) {
		case 35: goto tr772;
		case 47: goto tr773;
		case 66: goto tr774;
		case 67: goto tr775;
		case 73: goto tr776;
		case 78: goto tr777;
		case 81: goto tr778;
		case 83: goto tr779;
		case 84: goto tr780;
		case 85: goto tr781;
		case 91: goto tr782;
		case 98: goto tr774;
		case 99: goto tr775;
		case 105: goto tr776;
		case 110: goto tr777;
		case 113: goto tr778;
		case 115: goto tr779;
		case 116: goto tr780;
		case 117: goto tr781;
	}
	goto tr692;
case 280:
	if ( (*( sm->p)) == 93 )
		goto tr107;
	goto tr317;
case 281:
	if ( (*( sm->p)) == 93 )
		goto tr319;
	goto tr318;
case 282:
	switch( (*( sm->p)) ) {
		case 66: goto tr320;
		case 67: goto tr321;
		case 73: goto tr322;
		case 81: goto tr323;
		case 83: goto tr324;
		case 84: goto tr325;
		case 85: goto tr326;
		case 98: goto tr320;
		case 99: goto tr321;
		case 105: goto tr322;
		case 113: goto tr323;
		case 115: goto tr324;
		case 116: goto tr325;
		case 117: goto tr326;
	}
	goto tr107;
case 283:
	if ( (*( sm->p)) == 93 )
		goto tr327;
	goto tr107;
case 284:
	switch( (*( sm->p)) ) {
		case 79: goto tr328;
		case 111: goto tr328;
	}
	goto tr107;
case 285:
	switch( (*( sm->p)) ) {
		case 68: goto tr329;
		case 76: goto tr330;
		case 100: goto tr329;
		case 108: goto tr330;
	}
	goto tr107;
case 286:
	switch( (*( sm->p)) ) {
		case 69: goto tr331;
		case 101: goto tr331;
	}
	goto tr107;
case 287:
	if ( (*( sm->p)) == 93 )
		goto tr332;
	goto tr107;
case 602:
	if ( (*( sm->p)) == 32 )
		goto tr332;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto tr332;
	goto tr783;
case 288:
	switch( (*( sm->p)) ) {
		case 79: goto tr333;
		case 111: goto tr333;
	}
	goto tr107;
case 289:
	switch( (*( sm->p)) ) {
		case 82: goto tr334;
		case 114: goto tr334;
	}
	goto tr107;
case 290:
	if ( (*( sm->p)) == 93 )
		goto tr335;
	goto tr107;
case 291:
	if ( (*( sm->p)) == 93 )
		goto tr336;
	goto tr107;
case 292:
	switch( (*( sm->p)) ) {
		case 85: goto tr337;
		case 117: goto tr337;
	}
	goto tr107;
case 293:
	switch( (*( sm->p)) ) {
		case 79: goto tr338;
		case 111: goto tr338;
	}
	goto tr107;
case 294:
	switch( (*( sm->p)) ) {
		case 84: goto tr339;
		case 116: goto tr339;
	}
	goto tr107;
case 295:
	switch( (*( sm->p)) ) {
		case 69: goto tr340;
		case 101: goto tr340;
	}
	goto tr107;
case 296:
	if ( (*( sm->p)) == 93 )
		goto tr341;
	goto tr107;
case 603:
	if ( (*( sm->p)) == 32 )
		goto tr341;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto tr341;
	goto tr784;
case 297:
	switch( (*( sm->p)) ) {
		case 69: goto tr342;
		case 80: goto tr343;
		case 85: goto tr344;
		case 93: goto tr345;
		case 101: goto tr342;
		case 112: goto tr343;
		case 117: goto tr344;
	}
	goto tr107;
case 298:
	switch( (*( sm->p)) ) {
		case 67: goto tr346;
		case 99: goto tr346;
	}
	goto tr107;
case 299:
	switch( (*( sm->p)) ) {
		case 84: goto tr347;
		case 116: goto tr347;
	}
	goto tr107;
case 300:
	switch( (*( sm->p)) ) {
		case 73: goto tr348;
		case 105: goto tr348;
	}
	goto tr107;
case 301:
	switch( (*( sm->p)) ) {
		case 79: goto tr349;
		case 111: goto tr349;
	}
	goto tr107;
case 302:
	switch( (*( sm->p)) ) {
		case 78: goto tr350;
		case 110: goto tr350;
	}
	goto tr107;
case 303:
	if ( (*( sm->p)) == 93 )
		goto tr351;
	goto tr107;
case 304:
	switch( (*( sm->p)) ) {
		case 79: goto tr352;
		case 111: goto tr352;
	}
	goto tr107;
case 305:
	switch( (*( sm->p)) ) {
		case 73: goto tr353;
		case 105: goto tr353;
	}
	goto tr107;
case 306:
	switch( (*( sm->p)) ) {
		case 76: goto tr354;
		case 108: goto tr354;
	}
	goto tr107;
case 307:
	switch( (*( sm->p)) ) {
		case 69: goto tr355;
		case 101: goto tr355;
	}
	goto tr107;
case 308:
	switch( (*( sm->p)) ) {
		case 82: goto tr356;
		case 114: goto tr356;
	}
	goto tr107;
case 309:
	switch( (*( sm->p)) ) {
		case 83: goto tr357;
		case 93: goto tr358;
		case 115: goto tr357;
	}
	goto tr107;
case 310:
	if ( (*( sm->p)) == 93 )
		goto tr358;
	goto tr107;
case 311:
	switch( (*( sm->p)) ) {
		case 66: goto tr359;
		case 80: goto tr360;
		case 98: goto tr359;
		case 112: goto tr360;
	}
	goto tr107;
case 312:
	if ( (*( sm->p)) == 93 )
		goto tr361;
	goto tr107;
case 313:
	if ( (*( sm->p)) == 93 )
		goto tr362;
	goto tr107;
case 314:
	switch( (*( sm->p)) ) {
		case 65: goto tr363;
		case 68: goto tr364;
		case 72: goto tr365;
		case 78: goto tr366;
		case 97: goto tr363;
		case 100: goto tr364;
		case 104: goto tr365;
		case 110: goto tr366;
	}
	goto tr107;
case 315:
	switch( (*( sm->p)) ) {
		case 66: goto tr367;
		case 98: goto tr367;
	}
	goto tr107;
case 316:
	switch( (*( sm->p)) ) {
		case 76: goto tr368;
		case 108: goto tr368;
	}
	goto tr107;
case 317:
	switch( (*( sm->p)) ) {
		case 69: goto tr369;
		case 101: goto tr369;
	}
	goto tr107;
case 318:
	if ( (*( sm->p)) == 93 )
		goto tr370;
	goto tr107;
case 604:
	if ( (*( sm->p)) == 32 )
		goto tr370;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto tr370;
	goto tr785;
case 319:
	if ( (*( sm->p)) == 93 )
		goto tr371;
	goto tr107;
case 320:
	if ( (*( sm->p)) == 93 )
		goto tr372;
	goto tr107;
case 321:
	if ( (*( sm->p)) == 93 )
		goto tr373;
	goto tr107;
case 322:
	if ( (*( sm->p)) == 93 )
		goto tr374;
	goto tr107;
case 323:
	if ( (*( sm->p)) == 93 )
		goto tr375;
	goto tr107;
case 324:
	switch( (*( sm->p)) ) {
		case 79: goto tr376;
		case 111: goto tr376;
	}
	goto tr107;
case 325:
	switch( (*( sm->p)) ) {
		case 68: goto tr377;
		case 76: goto tr378;
		case 100: goto tr377;
		case 108: goto tr378;
	}
	goto tr107;
case 326:
	switch( (*( sm->p)) ) {
		case 69: goto tr379;
		case 101: goto tr379;
	}
	goto tr107;
case 327:
	if ( (*( sm->p)) == 93 )
		goto tr380;
	goto tr107;
case 328:
	switch( (*( sm->p)) ) {
		case 79: goto tr381;
		case 111: goto tr381;
	}
	goto tr107;
case 329:
	switch( (*( sm->p)) ) {
		case 82: goto tr382;
		case 114: goto tr382;
	}
	goto tr107;
case 330:
	if ( (*( sm->p)) == 61 )
		goto tr383;
	goto tr107;
case 331:
	switch( (*( sm->p)) ) {
		case 35: goto tr384;
		case 65: goto tr385;
		case 67: goto tr386;
		case 73: goto tr387;
		case 76: goto tr388;
		case 77: goto tr389;
		case 83: goto tr390;
		case 97: goto tr391;
		case 99: goto tr393;
		case 105: goto tr394;
		case 108: goto tr395;
		case 109: goto tr396;
		case 115: goto tr397;
	}
	if ( 98 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr392;
	goto tr107;
case 332:
	if ( (*( sm->p)) < 65 ) {
		if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
			goto tr398;
	} else if ( (*( sm->p)) > 70 ) {
		if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 102 )
			goto tr398;
	} else
		goto tr398;
	goto tr107;
case 333:
	if ( (*( sm->p)) < 65 ) {
		if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
			goto tr399;
	} else if ( (*( sm->p)) > 70 ) {
		if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 102 )
			goto tr399;
	} else
		goto tr399;
	goto tr107;
case 334:
	if ( (*( sm->p)) < 65 ) {
		if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
			goto tr400;
	} else if ( (*( sm->p)) > 70 ) {
		if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 102 )
			goto tr400;
	} else
		goto tr400;
	goto tr107;
case 335:
	if ( (*( sm->p)) == 93 )
		goto tr402;
	if ( (*( sm->p)) < 65 ) {
		if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
			goto tr401;
	} else if ( (*( sm->p)) > 70 ) {
		if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 102 )
			goto tr401;
	} else
		goto tr401;
	goto tr107;
case 336:
	if ( (*( sm->p)) == 93 )
		goto tr402;
	if ( (*( sm->p)) < 65 ) {
		if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
			goto tr403;
	} else if ( (*( sm->p)) > 70 ) {
		if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 102 )
			goto tr403;
	} else
		goto tr403;
	goto tr107;
case 337:
	if ( (*( sm->p)) == 93 )
		goto tr402;
	if ( (*( sm->p)) < 65 ) {
		if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
			goto tr404;
	} else if ( (*( sm->p)) > 70 ) {
		if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 102 )
			goto tr404;
	} else
		goto tr404;
	goto tr107;
case 338:
	if ( (*( sm->p)) == 93 )
		goto tr402;
	goto tr107;
case 339:
	switch( (*( sm->p)) ) {
		case 82: goto tr405;
		case 114: goto tr405;
	}
	goto tr107;
case 340:
	switch( (*( sm->p)) ) {
		case 84: goto tr406;
		case 116: goto tr406;
	}
	goto tr107;
case 341:
	switch( (*( sm->p)) ) {
		case 73: goto tr407;
		case 93: goto tr408;
		case 105: goto tr407;
	}
	goto tr107;
case 342:
	switch( (*( sm->p)) ) {
		case 83: goto tr409;
		case 115: goto tr409;
	}
	goto tr107;
case 343:
	switch( (*( sm->p)) ) {
		case 84: goto tr410;
		case 116: goto tr410;
	}
	goto tr107;
case 344:
	if ( (*( sm->p)) == 93 )
		goto tr408;
	goto tr107;
case 345:
	switch( (*( sm->p)) ) {
		case 72: goto tr411;
		case 79: goto tr412;
		case 104: goto tr411;
		case 111: goto tr412;
	}
	goto tr107;
case 346:
	switch( (*( sm->p)) ) {
		case 65: goto tr413;
		case 97: goto tr413;
	}
	goto tr107;
case 347:
	switch( (*( sm->p)) ) {
		case 82: goto tr414;
		case 114: goto tr414;
	}
	goto tr107;
case 348:
	switch( (*( sm->p)) ) {
		case 65: goto tr415;
		case 93: goto tr408;
		case 97: goto tr415;
	}
	goto tr107;
case 349:
	switch( (*( sm->p)) ) {
		case 67: goto tr416;
		case 99: goto tr416;
	}
	goto tr107;
case 350:
	switch( (*( sm->p)) ) {
		case 84: goto tr417;
		case 116: goto tr417;
	}
	goto tr107;
case 351:
	switch( (*( sm->p)) ) {
		case 69: goto tr418;
		case 101: goto tr418;
	}
	goto tr107;
case 352:
	switch( (*( sm->p)) ) {
		case 82: goto tr410;
		case 114: goto tr410;
	}
	goto tr107;
case 353:
	switch( (*( sm->p)) ) {
		case 80: goto tr419;
		case 112: goto tr419;
	}
	goto tr107;
case 354:
	switch( (*( sm->p)) ) {
		case 89: goto tr420;
		case 121: goto tr420;
	}
	goto tr107;
case 355:
	switch( (*( sm->p)) ) {
		case 82: goto tr421;
		case 93: goto tr408;
		case 114: goto tr421;
	}
	goto tr107;
case 356:
	switch( (*( sm->p)) ) {
		case 73: goto tr422;
		case 105: goto tr422;
	}
	goto tr107;
case 357:
	switch( (*( sm->p)) ) {
		case 71: goto tr423;
		case 103: goto tr423;
	}
	goto tr107;
case 358:
	switch( (*( sm->p)) ) {
		case 72: goto tr409;
		case 104: goto tr409;
	}
	goto tr107;
case 359:
	switch( (*( sm->p)) ) {
		case 78: goto tr424;
		case 110: goto tr424;
	}
	goto tr107;
case 360:
	switch( (*( sm->p)) ) {
		case 86: goto tr425;
		case 118: goto tr425;
	}
	goto tr107;
case 361:
	switch( (*( sm->p)) ) {
		case 65: goto tr426;
		case 93: goto tr408;
		case 97: goto tr426;
	}
	goto tr107;
case 362:
	switch( (*( sm->p)) ) {
		case 76: goto tr427;
		case 108: goto tr427;
	}
	goto tr107;
case 363:
	switch( (*( sm->p)) ) {
		case 73: goto tr428;
		case 105: goto tr428;
	}
	goto tr107;
case 364:
	switch( (*( sm->p)) ) {
		case 68: goto tr410;
		case 100: goto tr410;
	}
	goto tr107;
case 365:
	switch( (*( sm->p)) ) {
		case 79: goto tr429;
		case 111: goto tr429;
	}
	goto tr107;
case 366:
	switch( (*( sm->p)) ) {
		case 82: goto tr430;
		case 114: goto tr430;
	}
	goto tr107;
case 367:
	switch( (*( sm->p)) ) {
		case 69: goto tr410;
		case 101: goto tr410;
	}
	goto tr107;
case 368:
	switch( (*( sm->p)) ) {
		case 69: goto tr431;
		case 101: goto tr431;
	}
	goto tr107;
case 369:
	switch( (*( sm->p)) ) {
		case 84: goto tr432;
		case 116: goto tr432;
	}
	goto tr107;
case 370:
	switch( (*( sm->p)) ) {
		case 65: goto tr410;
		case 97: goto tr410;
	}
	goto tr107;
case 371:
	switch( (*( sm->p)) ) {
		case 80: goto tr433;
		case 112: goto tr433;
	}
	goto tr107;
case 372:
	switch( (*( sm->p)) ) {
		case 69: goto tr434;
		case 101: goto tr434;
	}
	goto tr107;
case 373:
	switch( (*( sm->p)) ) {
		case 67: goto tr435;
		case 99: goto tr435;
	}
	goto tr107;
case 374:
	switch( (*( sm->p)) ) {
		case 73: goto tr436;
		case 93: goto tr408;
		case 105: goto tr436;
	}
	goto tr107;
case 375:
	switch( (*( sm->p)) ) {
		case 69: goto tr437;
		case 101: goto tr437;
	}
	goto tr107;
case 376:
	switch( (*( sm->p)) ) {
		case 83: goto tr410;
		case 115: goto tr410;
	}
	goto tr107;
case 377:
	switch( (*( sm->p)) ) {
		case 82: goto tr405;
		case 93: goto tr402;
		case 114: goto tr439;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 378:
	if ( (*( sm->p)) == 93 )
		goto tr402;
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 379:
	switch( (*( sm->p)) ) {
		case 84: goto tr406;
		case 93: goto tr402;
		case 116: goto tr440;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 380:
	switch( (*( sm->p)) ) {
		case 73: goto tr407;
		case 93: goto tr408;
		case 105: goto tr441;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 381:
	switch( (*( sm->p)) ) {
		case 83: goto tr409;
		case 93: goto tr402;
		case 115: goto tr442;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 382:
	switch( (*( sm->p)) ) {
		case 84: goto tr410;
		case 93: goto tr402;
		case 116: goto tr443;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 383:
	if ( (*( sm->p)) == 93 )
		goto tr408;
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 384:
	switch( (*( sm->p)) ) {
		case 72: goto tr411;
		case 79: goto tr412;
		case 93: goto tr402;
		case 104: goto tr444;
		case 111: goto tr445;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 385:
	switch( (*( sm->p)) ) {
		case 65: goto tr413;
		case 93: goto tr402;
		case 97: goto tr446;
	}
	if ( 98 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 386:
	switch( (*( sm->p)) ) {
		case 82: goto tr414;
		case 93: goto tr402;
		case 114: goto tr447;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 387:
	switch( (*( sm->p)) ) {
		case 65: goto tr415;
		case 93: goto tr408;
		case 97: goto tr448;
	}
	if ( 98 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 388:
	switch( (*( sm->p)) ) {
		case 67: goto tr416;
		case 93: goto tr402;
		case 99: goto tr449;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 389:
	switch( (*( sm->p)) ) {
		case 84: goto tr417;
		case 93: goto tr402;
		case 116: goto tr450;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 390:
	switch( (*( sm->p)) ) {
		case 69: goto tr418;
		case 93: goto tr402;
		case 101: goto tr451;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 391:
	switch( (*( sm->p)) ) {
		case 82: goto tr410;
		case 93: goto tr402;
		case 114: goto tr443;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 392:
	switch( (*( sm->p)) ) {
		case 80: goto tr419;
		case 93: goto tr402;
		case 112: goto tr452;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 393:
	switch( (*( sm->p)) ) {
		case 89: goto tr420;
		case 93: goto tr402;
		case 121: goto tr453;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 394:
	switch( (*( sm->p)) ) {
		case 82: goto tr421;
		case 93: goto tr408;
		case 114: goto tr454;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 395:
	switch( (*( sm->p)) ) {
		case 73: goto tr422;
		case 93: goto tr402;
		case 105: goto tr455;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 396:
	switch( (*( sm->p)) ) {
		case 71: goto tr423;
		case 93: goto tr402;
		case 103: goto tr456;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 397:
	switch( (*( sm->p)) ) {
		case 72: goto tr409;
		case 93: goto tr402;
		case 104: goto tr442;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 398:
	switch( (*( sm->p)) ) {
		case 78: goto tr424;
		case 93: goto tr402;
		case 110: goto tr457;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 399:
	switch( (*( sm->p)) ) {
		case 86: goto tr425;
		case 93: goto tr402;
		case 118: goto tr458;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 400:
	switch( (*( sm->p)) ) {
		case 65: goto tr426;
		case 93: goto tr408;
		case 97: goto tr459;
	}
	if ( 98 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 401:
	switch( (*( sm->p)) ) {
		case 76: goto tr427;
		case 93: goto tr402;
		case 108: goto tr460;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 402:
	switch( (*( sm->p)) ) {
		case 73: goto tr428;
		case 93: goto tr402;
		case 105: goto tr461;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 403:
	switch( (*( sm->p)) ) {
		case 68: goto tr410;
		case 93: goto tr402;
		case 100: goto tr443;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 404:
	switch( (*( sm->p)) ) {
		case 79: goto tr429;
		case 93: goto tr402;
		case 111: goto tr462;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 405:
	switch( (*( sm->p)) ) {
		case 82: goto tr430;
		case 93: goto tr402;
		case 114: goto tr463;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 406:
	switch( (*( sm->p)) ) {
		case 69: goto tr410;
		case 93: goto tr402;
		case 101: goto tr443;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 407:
	switch( (*( sm->p)) ) {
		case 69: goto tr431;
		case 93: goto tr402;
		case 101: goto tr464;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 408:
	switch( (*( sm->p)) ) {
		case 84: goto tr432;
		case 93: goto tr402;
		case 116: goto tr465;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 409:
	switch( (*( sm->p)) ) {
		case 65: goto tr410;
		case 93: goto tr402;
		case 97: goto tr443;
	}
	if ( 98 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 410:
	switch( (*( sm->p)) ) {
		case 80: goto tr433;
		case 93: goto tr402;
		case 112: goto tr466;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 411:
	switch( (*( sm->p)) ) {
		case 69: goto tr434;
		case 93: goto tr402;
		case 101: goto tr467;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 412:
	switch( (*( sm->p)) ) {
		case 67: goto tr435;
		case 93: goto tr402;
		case 99: goto tr468;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 413:
	switch( (*( sm->p)) ) {
		case 73: goto tr436;
		case 93: goto tr408;
		case 105: goto tr469;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 414:
	switch( (*( sm->p)) ) {
		case 69: goto tr437;
		case 93: goto tr402;
		case 101: goto tr470;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 415:
	switch( (*( sm->p)) ) {
		case 83: goto tr410;
		case 93: goto tr402;
		case 115: goto tr443;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr438;
	goto tr107;
case 416:
	if ( (*( sm->p)) == 93 )
		goto tr471;
	goto tr107;
case 417:
	switch( (*( sm->p)) ) {
		case 79: goto tr472;
		case 111: goto tr472;
	}
	goto tr107;
case 418:
	switch( (*( sm->p)) ) {
		case 68: goto tr473;
		case 100: goto tr473;
	}
	goto tr107;
case 419:
	switch( (*( sm->p)) ) {
		case 84: goto tr474;
		case 116: goto tr474;
	}
	goto tr107;
case 420:
	switch( (*( sm->p)) ) {
		case 69: goto tr475;
		case 101: goto tr475;
	}
	goto tr107;
case 421:
	switch( (*( sm->p)) ) {
		case 88: goto tr476;
		case 120: goto tr476;
	}
	goto tr107;
case 422:
	switch( (*( sm->p)) ) {
		case 84: goto tr477;
		case 116: goto tr477;
	}
	goto tr107;
case 423:
	if ( (*( sm->p)) == 93 )
		goto tr478;
	goto tr107;
case 424:
	switch( (*( sm->p)) ) {
		case 85: goto tr479;
		case 117: goto tr479;
	}
	goto tr107;
case 425:
	switch( (*( sm->p)) ) {
		case 79: goto tr480;
		case 111: goto tr480;
	}
	goto tr107;
case 426:
	switch( (*( sm->p)) ) {
		case 84: goto tr481;
		case 116: goto tr481;
	}
	goto tr107;
case 427:
	switch( (*( sm->p)) ) {
		case 69: goto tr482;
		case 101: goto tr482;
	}
	goto tr107;
case 428:
	if ( (*( sm->p)) == 93 )
		goto tr483;
	goto tr107;
case 429:
	switch( (*( sm->p)) ) {
		case 69: goto tr484;
		case 80: goto tr485;
		case 85: goto tr486;
		case 93: goto tr487;
		case 101: goto tr484;
		case 112: goto tr485;
		case 117: goto tr486;
	}
	goto tr107;
case 430:
	switch( (*( sm->p)) ) {
		case 67: goto tr488;
		case 99: goto tr488;
	}
	goto tr107;
case 431:
	switch( (*( sm->p)) ) {
		case 84: goto tr489;
		case 116: goto tr489;
	}
	goto tr107;
case 432:
	switch( (*( sm->p)) ) {
		case 73: goto tr490;
		case 105: goto tr490;
	}
	goto tr107;
case 433:
	switch( (*( sm->p)) ) {
		case 79: goto tr491;
		case 111: goto tr491;
	}
	goto tr107;
case 434:
	switch( (*( sm->p)) ) {
		case 78: goto tr492;
		case 110: goto tr492;
	}
	goto tr107;
case 435:
	switch( (*( sm->p)) ) {
		case 44: goto tr493;
		case 61: goto tr494;
		case 93: goto tr495;
	}
	goto tr107;
case 436:
	switch( (*( sm->p)) ) {
		case 69: goto tr496;
		case 101: goto tr496;
	}
	goto tr107;
case 437:
	switch( (*( sm->p)) ) {
		case 88: goto tr497;
		case 120: goto tr497;
	}
	goto tr107;
case 438:
	switch( (*( sm->p)) ) {
		case 80: goto tr498;
		case 112: goto tr498;
	}
	goto tr107;
case 439:
	switch( (*( sm->p)) ) {
		case 65: goto tr499;
		case 97: goto tr499;
	}
	goto tr107;
case 440:
	switch( (*( sm->p)) ) {
		case 78: goto tr500;
		case 110: goto tr500;
	}
	goto tr107;
case 441:
	switch( (*( sm->p)) ) {
		case 68: goto tr501;
		case 100: goto tr501;
	}
	goto tr107;
case 442:
	switch( (*( sm->p)) ) {
		case 69: goto tr502;
		case 101: goto tr502;
	}
	goto tr107;
case 443:
	switch( (*( sm->p)) ) {
		case 68: goto tr503;
		case 100: goto tr503;
	}
	goto tr107;
case 444:
	switch( (*( sm->p)) ) {
		case 61: goto tr504;
		case 93: goto tr505;
	}
	goto tr107;
case 445:
	if ( (*( sm->p)) == 93 )
		goto tr107;
	goto tr506;
case 446:
	if ( (*( sm->p)) == 93 )
		goto tr508;
	goto tr507;
case 447:
	if ( (*( sm->p)) == 93 )
		goto tr107;
	goto tr509;
case 448:
	if ( (*( sm->p)) == 93 )
		goto tr511;
	goto tr510;
case 449:
	switch( (*( sm->p)) ) {
		case 79: goto tr512;
		case 111: goto tr512;
	}
	goto tr107;
case 450:
	switch( (*( sm->p)) ) {
		case 73: goto tr513;
		case 105: goto tr513;
	}
	goto tr107;
case 451:
	switch( (*( sm->p)) ) {
		case 76: goto tr514;
		case 108: goto tr514;
	}
	goto tr107;
case 452:
	switch( (*( sm->p)) ) {
		case 69: goto tr515;
		case 101: goto tr515;
	}
	goto tr107;
case 453:
	switch( (*( sm->p)) ) {
		case 82: goto tr516;
		case 114: goto tr516;
	}
	goto tr107;
case 454:
	switch( (*( sm->p)) ) {
		case 83: goto tr517;
		case 93: goto tr518;
		case 115: goto tr517;
	}
	goto tr107;
case 455:
	if ( (*( sm->p)) == 93 )
		goto tr518;
	goto tr107;
case 456:
	switch( (*( sm->p)) ) {
		case 66: goto tr519;
		case 80: goto tr520;
		case 98: goto tr519;
		case 112: goto tr520;
	}
	goto tr107;
case 457:
	if ( (*( sm->p)) == 93 )
		goto tr521;
	goto tr107;
case 458:
	if ( (*( sm->p)) == 93 )
		goto tr522;
	goto tr107;
case 459:
	switch( (*( sm->p)) ) {
		case 65: goto tr523;
		case 78: goto tr524;
		case 97: goto tr523;
		case 110: goto tr524;
	}
	goto tr107;
case 460:
	switch( (*( sm->p)) ) {
		case 66: goto tr525;
		case 98: goto tr525;
	}
	goto tr107;
case 461:
	switch( (*( sm->p)) ) {
		case 76: goto tr526;
		case 108: goto tr526;
	}
	goto tr107;
case 462:
	switch( (*( sm->p)) ) {
		case 69: goto tr527;
		case 101: goto tr527;
	}
	goto tr107;
case 463:
	if ( (*( sm->p)) == 93 )
		goto tr528;
	goto tr107;
case 464:
	if ( (*( sm->p)) == 93 )
		goto tr529;
	goto tr107;
case 465:
	if ( (*( sm->p)) == 93 )
		goto tr530;
	goto tr107;
case 466:
	switch( (*( sm->p)) ) {
		case 93: goto tr107;
		case 124: goto tr532;
	}
	goto tr531;
case 467:
	switch( (*( sm->p)) ) {
		case 93: goto tr534;
		case 124: goto tr535;
	}
	goto tr533;
case 468:
	if ( (*( sm->p)) == 93 )
		goto tr536;
	goto tr107;
case 469:
	switch( (*( sm->p)) ) {
		case 93: goto tr107;
		case 124: goto tr107;
	}
	goto tr537;
case 470:
	switch( (*( sm->p)) ) {
		case 93: goto tr539;
		case 124: goto tr107;
	}
	goto tr538;
case 471:
	if ( (*( sm->p)) == 93 )
		goto tr540;
	goto tr107;
case 472:
	switch( (*( sm->p)) ) {
		case 93: goto tr534;
		case 124: goto tr107;
	}
	goto tr541;
case 605:
	if ( (*( sm->p)) == 96 )
		goto tr786;
	goto tr692;
case 606:
	if ( (*( sm->p)) == 116 )
		goto tr787;
	goto tr692;
case 473:
	if ( (*( sm->p)) == 116 )
		goto tr542;
	goto tr107;
case 474:
	if ( (*( sm->p)) == 112 )
		goto tr543;
	goto tr107;
case 475:
	switch( (*( sm->p)) ) {
		case 58: goto tr544;
		case 115: goto tr545;
	}
	goto tr107;
case 476:
	if ( (*( sm->p)) == 47 )
		goto tr546;
	goto tr107;
case 477:
	if ( (*( sm->p)) == 47 )
		goto tr547;
	goto tr107;
case 478:
	if ( (*( sm->p)) < -32 ) {
		if ( -62 <= (*( sm->p)) && (*( sm->p)) <= -33 )
			goto tr548;
	} else if ( (*( sm->p)) > -17 ) {
		if ( (*( sm->p)) > -12 ) {
			if ( 33 <= (*( sm->p)) && (*( sm->p)) <= 126 )
				goto tr551;
		} else if ( (*( sm->p)) >= -16 )
			goto tr550;
	} else
		goto tr549;
	goto tr107;
case 479:
	if ( (*( sm->p)) <= -65 )
		goto tr551;
	goto tr100;
case 607:
	if ( (*( sm->p)) < -32 ) {
		if ( -62 <= (*( sm->p)) && (*( sm->p)) <= -33 )
			goto tr548;
	} else if ( (*( sm->p)) > -17 ) {
		if ( (*( sm->p)) > -12 ) {
			if ( 33 <= (*( sm->p)) && (*( sm->p)) <= 126 )
				goto tr551;
		} else if ( (*( sm->p)) >= -16 )
			goto tr550;
	} else
		goto tr549;
	goto tr788;
case 480:
	if ( (*( sm->p)) <= -65 )
		goto tr548;
	goto tr100;
case 481:
	if ( (*( sm->p)) <= -65 )
		goto tr549;
	goto tr100;
case 482:
	if ( (*( sm->p)) == 58 )
		goto tr544;
	goto tr107;
case 608:
	if ( (*( sm->p)) == 123 )
		goto tr789;
	goto tr692;
case 483:
	switch( (*( sm->p)) ) {
		case 124: goto tr553;
		case 125: goto tr107;
	}
	goto tr552;
case 484:
	switch( (*( sm->p)) ) {
		case 124: goto tr555;
		case 125: goto tr556;
	}
	goto tr554;
case 485:
	if ( 124 <= (*( sm->p)) && (*( sm->p)) <= 125 )
		goto tr107;
	goto tr557;
case 486:
	switch( (*( sm->p)) ) {
		case 124: goto tr107;
		case 125: goto tr559;
	}
	goto tr558;
case 487:
	if ( (*( sm->p)) == 125 )
		goto tr560;
	goto tr107;
case 488:
	if ( (*( sm->p)) == 125 )
		goto tr561;
	goto tr107;
case 489:
	switch( (*( sm->p)) ) {
		case 124: goto tr107;
		case 125: goto tr556;
	}
	goto tr562;
case 609:
	switch( (*( sm->p)) ) {
		case 92: goto tr791;
		case 96: goto tr792;
	}
	goto tr790;
case 610:
	if ( (*( sm->p)) == 96 )
		goto tr794;
	goto tr793;
case 611:
	if ( (*( sm->p)) == 91 )
		goto tr796;
	goto tr795;
case 612:
	if ( (*( sm->p)) == 47 )
		goto tr798;
	goto tr797;
case 490:
	switch( (*( sm->p)) ) {
		case 67: goto tr564;
		case 99: goto tr564;
	}
	goto tr563;
case 491:
	switch( (*( sm->p)) ) {
		case 79: goto tr565;
		case 111: goto tr565;
	}
	goto tr563;
case 492:
	switch( (*( sm->p)) ) {
		case 68: goto tr566;
		case 100: goto tr566;
	}
	goto tr563;
case 493:
	switch( (*( sm->p)) ) {
		case 69: goto tr567;
		case 101: goto tr567;
	}
	goto tr563;
case 494:
	if ( (*( sm->p)) == 93 )
		goto tr568;
	goto tr563;
case 613:
	if ( (*( sm->p)) == 91 )
		goto tr800;
	goto tr799;
case 614:
	if ( (*( sm->p)) == 47 )
		goto tr802;
	goto tr801;
case 495:
	switch( (*( sm->p)) ) {
		case 78: goto tr570;
		case 110: goto tr570;
	}
	goto tr569;
case 496:
	switch( (*( sm->p)) ) {
		case 79: goto tr571;
		case 111: goto tr571;
	}
	goto tr569;
case 497:
	switch( (*( sm->p)) ) {
		case 68: goto tr572;
		case 100: goto tr572;
	}
	goto tr569;
case 498:
	switch( (*( sm->p)) ) {
		case 84: goto tr573;
		case 116: goto tr573;
	}
	goto tr569;
case 499:
	switch( (*( sm->p)) ) {
		case 69: goto tr574;
		case 101: goto tr574;
	}
	goto tr569;
case 500:
	switch( (*( sm->p)) ) {
		case 88: goto tr575;
		case 120: goto tr575;
	}
	goto tr569;
case 501:
	switch( (*( sm->p)) ) {
		case 84: goto tr576;
		case 116: goto tr576;
	}
	goto tr569;
case 502:
	if ( (*( sm->p)) == 93 )
		goto tr577;
	goto tr569;
case 615:
	if ( (*( sm->p)) == 91 )
		goto tr804;
	goto tr803;
case 616:
	switch( (*( sm->p)) ) {
		case 47: goto tr806;
		case 84: goto tr807;
		case 116: goto tr807;
	}
	goto tr805;
case 503:
	switch( (*( sm->p)) ) {
		case 84: goto tr579;
		case 116: goto tr579;
	}
	goto tr578;
case 504:
	switch( (*( sm->p)) ) {
		case 65: goto tr580;
		case 66: goto tr581;
		case 72: goto tr582;
		case 82: goto tr583;
		case 97: goto tr580;
		case 98: goto tr581;
		case 104: goto tr582;
		case 114: goto tr583;
	}
	goto tr578;
case 505:
	switch( (*( sm->p)) ) {
		case 66: goto tr584;
		case 98: goto tr584;
	}
	goto tr578;
case 506:
	switch( (*( sm->p)) ) {
		case 76: goto tr585;
		case 108: goto tr585;
	}
	goto tr578;
case 507:
	switch( (*( sm->p)) ) {
		case 69: goto tr586;
		case 101: goto tr586;
	}
	goto tr578;
case 508:
	if ( (*( sm->p)) == 93 )
		goto tr587;
	goto tr578;
case 509:
	switch( (*( sm->p)) ) {
		case 79: goto tr588;
		case 111: goto tr588;
	}
	goto tr578;
case 510:
	switch( (*( sm->p)) ) {
		case 68: goto tr589;
		case 100: goto tr589;
	}
	goto tr578;
case 511:
	switch( (*( sm->p)) ) {
		case 89: goto tr590;
		case 121: goto tr590;
	}
	goto tr578;
case 512:
	if ( (*( sm->p)) == 93 )
		goto tr591;
	goto tr578;
case 513:
	switch( (*( sm->p)) ) {
		case 69: goto tr592;
		case 101: goto tr592;
	}
	goto tr578;
case 514:
	switch( (*( sm->p)) ) {
		case 65: goto tr593;
		case 97: goto tr593;
	}
	goto tr578;
case 515:
	switch( (*( sm->p)) ) {
		case 68: goto tr594;
		case 100: goto tr594;
	}
	goto tr578;
case 516:
	if ( (*( sm->p)) == 93 )
		goto tr595;
	goto tr578;
case 517:
	if ( (*( sm->p)) == 93 )
		goto tr596;
	goto tr578;
case 518:
	switch( (*( sm->p)) ) {
		case 66: goto tr597;
		case 68: goto tr598;
		case 72: goto tr599;
		case 82: goto tr600;
		case 98: goto tr597;
		case 100: goto tr598;
		case 104: goto tr599;
		case 114: goto tr600;
	}
	goto tr578;
case 519:
	switch( (*( sm->p)) ) {
		case 79: goto tr601;
		case 111: goto tr601;
	}
	goto tr578;
case 520:
	switch( (*( sm->p)) ) {
		case 68: goto tr602;
		case 100: goto tr602;
	}
	goto tr578;
case 521:
	switch( (*( sm->p)) ) {
		case 89: goto tr603;
		case 121: goto tr603;
	}
	goto tr578;
case 522:
	if ( (*( sm->p)) == 93 )
		goto tr604;
	goto tr578;
case 523:
	if ( (*( sm->p)) == 93 )
		goto tr605;
	goto tr578;
case 524:
	switch( (*( sm->p)) ) {
		case 69: goto tr606;
		case 93: goto tr607;
		case 101: goto tr606;
	}
	goto tr578;
case 525:
	switch( (*( sm->p)) ) {
		case 65: goto tr608;
		case 97: goto tr608;
	}
	goto tr578;
case 526:
	switch( (*( sm->p)) ) {
		case 68: goto tr609;
		case 100: goto tr609;
	}
	goto tr578;
case 527:
	if ( (*( sm->p)) == 93 )
		goto tr610;
	goto tr578;
case 528:
	if ( (*( sm->p)) == 93 )
		goto tr611;
	goto tr578;
case 617:
	switch( (*( sm->p)) ) {
		case 10: goto tr809;
		case 13: goto tr810;
		case 42: goto tr811;
	}
	goto tr808;
case 618:
	switch( (*( sm->p)) ) {
		case 10: goto tr613;
		case 13: goto tr812;
	}
	goto tr612;
case 529:
	if ( (*( sm->p)) == 10 )
		goto tr613;
	goto tr612;
case 619:
	if ( (*( sm->p)) == 10 )
		goto tr809;
	goto tr813;
case 620:
	switch( (*( sm->p)) ) {
		case 9: goto tr617;
		case 32: goto tr617;
		case 42: goto tr618;
	}
	goto tr813;
case 530:
	switch( (*( sm->p)) ) {
		case 9: goto tr616;
		case 10: goto tr614;
		case 13: goto tr614;
		case 32: goto tr616;
	}
	goto tr615;
case 621:
	switch( (*( sm->p)) ) {
		case 10: goto tr814;
		case 13: goto tr814;
	}
	goto tr815;
case 622:
	switch( (*( sm->p)) ) {
		case 9: goto tr616;
		case 10: goto tr814;
		case 13: goto tr814;
		case 32: goto tr616;
	}
	goto tr615;
case 531:
	switch( (*( sm->p)) ) {
		case 9: goto tr617;
		case 32: goto tr617;
		case 42: goto tr618;
	}
	goto tr614;
	}

	tr627:  sm->cs = 0; goto _again;
	tr5:  sm->cs = 1; goto f4;
	tr6:  sm->cs = 2; goto _again;
	tr631:  sm->cs = 3; goto f7;
	tr7:  sm->cs = 4; goto f4;
	tr10:  sm->cs = 5; goto _again;
	tr9:  sm->cs = 5; goto f3;
	tr636:  sm->cs = 6; goto _again;
	tr12:  sm->cs = 7; goto _again;
	tr13:  sm->cs = 8; goto _again;
	tr14:  sm->cs = 9; goto _again;
	tr15:  sm->cs = 10; goto _again;
	tr16:  sm->cs = 11; goto _again;
	tr17:  sm->cs = 12; goto _again;
	tr18:  sm->cs = 13; goto _again;
	tr19:  sm->cs = 14; goto _again;
	tr637:  sm->cs = 15; goto _again;
	tr21:  sm->cs = 16; goto _again;
	tr22:  sm->cs = 17; goto _again;
	tr23:  sm->cs = 18; goto _again;
	tr638:  sm->cs = 19; goto _again;
	tr25:  sm->cs = 20; goto _again;
	tr26:  sm->cs = 21; goto _again;
	tr27:  sm->cs = 22; goto _again;
	tr28:  sm->cs = 23; goto _again;
	tr29:  sm->cs = 24; goto _again;
	tr30:  sm->cs = 25; goto _again;
	tr639:  sm->cs = 26; goto _again;
	tr32:  sm->cs = 27; goto _again;
	tr33:  sm->cs = 28; goto _again;
	tr34:  sm->cs = 29; goto _again;
	tr35:  sm->cs = 30; goto _again;
	tr640:  sm->cs = 31; goto _again;
	tr37:  sm->cs = 32; goto _again;
	tr39:  sm->cs = 33; goto _again;
	tr40:  sm->cs = 34; goto _again;
	tr41:  sm->cs = 35; goto _again;
	tr42:  sm->cs = 36; goto _again;
	tr43:  sm->cs = 37; goto _again;
	tr44:  sm->cs = 38; goto _again;
	tr47:  sm->cs = 39; goto _again;
	tr48:  sm->cs = 40; goto _again;
	tr49:  sm->cs = 41; goto _again;
	tr50:  sm->cs = 42; goto _again;
	tr51:  sm->cs = 43; goto _again;
	tr52:  sm->cs = 44; goto _again;
	tr53:  sm->cs = 45; goto _again;
	tr54:  sm->cs = 46; goto _again;
	tr55:  sm->cs = 47; goto _again;
	tr58:  sm->cs = 48; goto _again;
	tr57:  sm->cs = 48; goto f7;
	tr45:  sm->cs = 49; goto _again;
	tr61:  sm->cs = 50; goto _again;
	tr60:  sm->cs = 50; goto f7;
	tr38:  sm->cs = 51; goto _again;
	tr63:  sm->cs = 52; goto _again;
	tr64:  sm->cs = 53; goto _again;
	tr65:  sm->cs = 54; goto _again;
	tr66:  sm->cs = 55; goto _again;
	tr67:  sm->cs = 56; goto _again;
	tr68:  sm->cs = 57; goto _again;
	tr641:  sm->cs = 58; goto _again;
	tr70:  sm->cs = 59; goto _again;
	tr72:  sm->cs = 60; goto _again;
	tr73:  sm->cs = 61; goto _again;
	tr74:  sm->cs = 62; goto _again;
	tr71:  sm->cs = 63; goto _again;
	tr656:  sm->cs = 64; goto _again;
	tr78:  sm->cs = 65; goto _again;
	tr79:  sm->cs = 66; goto _again;
	tr80:  sm->cs = 67; goto _again;
	tr84:  sm->cs = 68; goto _again;
	tr86:  sm->cs = 69; goto _again;
	tr87:  sm->cs = 70; goto _again;
	tr81:  sm->cs = 71; goto _again;
	tr657:  sm->cs = 72; goto _again;
	tr658:  sm->cs = 73; goto _again;
	tr659:  sm->cs = 74; goto _again;
	tr93:  sm->cs = 75; goto _again;
	tr95:  sm->cs = 76; goto _again;
	tr96:  sm->cs = 77; goto _again;
	tr660:  sm->cs = 78; goto _again;
	tr686:  sm->cs = 79; goto _again;
	tr104:  sm->cs = 80; goto _again;
	tr687:  sm->cs = 80; goto f7;
	tr103:  sm->cs = 81; goto f4;
	tr108:  sm->cs = 82; goto _again;
	tr693:  sm->cs = 82; goto f7;
	tr109:  sm->cs = 83; goto f4;
	tr110:  sm->cs = 84; goto _again;
	tr138:  sm->cs = 85; goto _again;
	tr111:  sm->cs = 85; goto f3;
	tr114:  sm->cs = 86; goto _again;
	tr115:  sm->cs = 87; goto _again;
	tr116:  sm->cs = 88; goto _again;
	tr112:  sm->cs = 89; goto _again;
	tr131:  sm->cs = 90; goto _again;
	tr118:  sm->cs = 90; goto f3;
	tr120:  sm->cs = 91; goto _again;
	tr123:  sm->cs = 92; goto f5;
	tr121:  sm->cs = 93; goto _again;
	tr122:  sm->cs = 94; goto _again;
	tr119:  sm->cs = 95; goto f3;
	tr125:  sm->cs = 96; goto _again;
	tr126:  sm->cs = 97; goto _again;
	tr127:  sm->cs = 98; goto _again;
	tr128:  sm->cs = 99; goto _again;
	tr130:  sm->cs = 100; goto _again;
	tr129:  sm->cs = 101; goto _again;
	tr113:  sm->cs = 102; goto f3;
	tr132:  sm->cs = 103; goto _again;
	tr133:  sm->cs = 104; goto _again;
	tr134:  sm->cs = 105; goto _again;
	tr135:  sm->cs = 106; goto _again;
	tr137:  sm->cs = 107; goto _again;
	tr136:  sm->cs = 108; goto _again;
	tr695:  sm->cs = 109; goto _again;
	tr144:  sm->cs = 110; goto _again;
	tr139:  sm->cs = 110; goto f7;
	tr143:  sm->cs = 111; goto _again;
	tr142:  sm->cs = 111; goto f7;
	tr145:  sm->cs = 112; goto _again;
	tr140:  sm->cs = 112; goto f7;
	tr146:  sm->cs = 113; goto _again;
	tr141:  sm->cs = 113; goto f7;
	tr696:  sm->cs = 114; goto _again;
	tr148:  sm->cs = 115; goto _again;
	tr149:  sm->cs = 116; goto _again;
	tr150:  sm->cs = 117; goto _again;
	tr151:  sm->cs = 118; goto _again;
	tr153:  sm->cs = 119; goto _again;
	tr154:  sm->cs = 120; goto _again;
	tr155:  sm->cs = 121; goto _again;
	tr158:  sm->cs = 122; goto _again;
	tr156:  sm->cs = 123; goto _again;
	tr157:  sm->cs = 124; goto _again;
	tr152:  sm->cs = 125; goto _again;
	tr161:  sm->cs = 126; goto _again;
	tr697:  sm->cs = 126; goto f7;
	tr162:  sm->cs = 127; goto _again;
	tr698:  sm->cs = 127; goto f7;
	tr702:  sm->cs = 128; goto _again;
	tr699:  sm->cs = 128; goto f7;
	tr705:  sm->cs = 129; goto _again;
	tr163:  sm->cs = 130; goto _again;
	tr164:  sm->cs = 131; goto _again;
	tr165:  sm->cs = 132; goto _again;
	tr166:  sm->cs = 133; goto _again;
	tr167:  sm->cs = 134; goto _again;
	tr706:  sm->cs = 135; goto _again;
	tr169:  sm->cs = 136; goto _again;
	tr170:  sm->cs = 137; goto _again;
	tr171:  sm->cs = 138; goto _again;
	tr172:  sm->cs = 139; goto _again;
	tr173:  sm->cs = 140; goto _again;
	tr174:  sm->cs = 141; goto _again;
	tr711:  sm->cs = 142; goto _again;
	tr176:  sm->cs = 143; goto _again;
	tr177:  sm->cs = 144; goto _again;
	tr178:  sm->cs = 145; goto _again;
	tr712:  sm->cs = 146; goto _again;
	tr180:  sm->cs = 147; goto _again;
	tr181:  sm->cs = 148; goto _again;
	tr182:  sm->cs = 149; goto _again;
	tr183:  sm->cs = 150; goto _again;
	tr713:  sm->cs = 151; goto _again;
	tr185:  sm->cs = 152; goto _again;
	tr186:  sm->cs = 153; goto _again;
	tr187:  sm->cs = 154; goto _again;
	tr720:  sm->cs = 155; goto _again;
	tr189:  sm->cs = 156; goto _again;
	tr190:  sm->cs = 157; goto _again;
	tr191:  sm->cs = 158; goto _again;
	tr192:  sm->cs = 159; goto _again;
	tr193:  sm->cs = 160; goto _again;
	tr194:  sm->cs = 161; goto _again;
	tr195:  sm->cs = 162; goto _again;
	tr723:  sm->cs = 163; goto _again;
	tr197:  sm->cs = 164; goto _again;
	tr198:  sm->cs = 165; goto _again;
	tr199:  sm->cs = 166; goto _again;
	tr200:  sm->cs = 167; goto _again;
	tr724:  sm->cs = 168; goto _again;
	tr202:  sm->cs = 169; goto _again;
	tr203:  sm->cs = 170; goto _again;
	tr204:  sm->cs = 171; goto _again;
	tr205:  sm->cs = 172; goto _again;
	tr206:  sm->cs = 173; goto _again;
	tr729:  sm->cs = 174; goto _again;
	tr208:  sm->cs = 175; goto _again;
	tr209:  sm->cs = 176; goto _again;
	tr210:  sm->cs = 177; goto _again;
	tr211:  sm->cs = 178; goto _again;
	tr212:  sm->cs = 179; goto _again;
	tr213:  sm->cs = 180; goto _again;
	tr214:  sm->cs = 181; goto _again;
	tr215:  sm->cs = 182; goto _again;
	tr216:  sm->cs = 183; goto _again;
	tr217:  sm->cs = 184; goto _again;
	tr218:  sm->cs = 185; goto _again;
	tr732:  sm->cs = 186; goto _again;
	tr220:  sm->cs = 187; goto _again;
	tr221:  sm->cs = 188; goto _again;
	tr222:  sm->cs = 189; goto _again;
	tr223:  sm->cs = 190; goto _again;
	tr224:  sm->cs = 191; goto _again;
	tr225:  sm->cs = 192; goto _again;
	tr226:  sm->cs = 193; goto _again;
	tr227:  sm->cs = 194; goto _again;
	tr228:  sm->cs = 195; goto _again;
	tr229:  sm->cs = 196; goto _again;
	tr735:  sm->cs = 197; goto _again;
	tr231:  sm->cs = 198; goto _again;
	tr232:  sm->cs = 199; goto _again;
	tr233:  sm->cs = 200; goto _again;
	tr234:  sm->cs = 201; goto _again;
	tr738:  sm->cs = 202; goto _again;
	tr236:  sm->cs = 203; goto _again;
	tr238:  sm->cs = 204; goto _again;
	tr239:  sm->cs = 205; goto _again;
	tr240:  sm->cs = 206; goto _again;
	tr237:  sm->cs = 207; goto _again;
	tr242:  sm->cs = 208; goto _again;
	tr243:  sm->cs = 209; goto _again;
	tr244:  sm->cs = 210; goto _again;
	tr245:  sm->cs = 211; goto _again;
	tr247:  sm->cs = 212; goto _again;
	tr248:  sm->cs = 213; goto _again;
	tr249:  sm->cs = 214; goto _again;
	tr250:  sm->cs = 215; goto _again;
	tr251:  sm->cs = 216; goto _again;
	tr252:  sm->cs = 217; goto _again;
	tr253:  sm->cs = 218; goto _again;
	tr254:  sm->cs = 219; goto _again;
	tr745:  sm->cs = 220; goto _again;
	tr256:  sm->cs = 221; goto _again;
	tr257:  sm->cs = 222; goto _again;
	tr258:  sm->cs = 223; goto _again;
	tr259:  sm->cs = 224; goto _again;
	tr260:  sm->cs = 225; goto _again;
	tr261:  sm->cs = 226; goto _again;
	tr748:  sm->cs = 227; goto _again;
	tr263:  sm->cs = 228; goto _again;
	tr264:  sm->cs = 229; goto _again;
	tr265:  sm->cs = 230; goto _again;
	tr751:  sm->cs = 231; goto _again;
	tr267:  sm->cs = 232; goto _again;
	tr268:  sm->cs = 233; goto _again;
	tr269:  sm->cs = 234; goto _again;
	tr270:  sm->cs = 235; goto _again;
	tr271:  sm->cs = 236; goto _again;
	tr272:  sm->cs = 237; goto _again;
	tr273:  sm->cs = 238; goto _again;
	tr274:  sm->cs = 239; goto _again;
	tr275:  sm->cs = 240; goto _again;
	tr276:  sm->cs = 241; goto _again;
	tr278:  sm->cs = 242; goto _again;
	tr279:  sm->cs = 243; goto _again;
	tr280:  sm->cs = 244; goto _again;
	tr281:  sm->cs = 245; goto _again;
	tr282:  sm->cs = 246; goto _again;
	tr283:  sm->cs = 247; goto _again;
	tr284:  sm->cs = 248; goto _again;
	tr752:  sm->cs = 249; goto _again;
	tr285:  sm->cs = 250; goto _again;
	tr286:  sm->cs = 251; goto _again;
	tr287:  sm->cs = 252; goto _again;
	tr288:  sm->cs = 253; goto _again;
	tr289:  sm->cs = 254; goto _again;
	tr753:  sm->cs = 255; goto _again;
	tr291:  sm->cs = 256; goto _again;
	tr292:  sm->cs = 257; goto _again;
	tr293:  sm->cs = 258; goto _again;
	tr294:  sm->cs = 259; goto _again;
	tr295:  sm->cs = 260; goto _again;
	tr296:  sm->cs = 261; goto _again;
	tr754:  sm->cs = 262; goto _again;
	tr298:  sm->cs = 263; goto _again;
	tr299:  sm->cs = 264; goto _again;
	tr300:  sm->cs = 265; goto _again;
	tr301:  sm->cs = 266; goto _again;
	tr302:  sm->cs = 267; goto _again;
	tr762:  sm->cs = 268; goto f4;
	tr305:  sm->cs = 269; goto _again;
	tr766:  sm->cs = 270; goto _again;
	tr307:  sm->cs = 271; goto _again;
	tr308:  sm->cs = 272; goto _again;
	tr309:  sm->cs = 273; goto _again;
	tr310:  sm->cs = 274; goto _again;
	tr769:  sm->cs = 275; goto _again;
	tr312:  sm->cs = 276; goto _again;
	tr313:  sm->cs = 277; goto _again;
	tr314:  sm->cs = 278; goto _again;
	tr315:  sm->cs = 279; goto _again;
	tr772:  sm->cs = 280; goto _again;
	tr318:  sm->cs = 281; goto _again;
	tr317:  sm->cs = 281; goto f7;
	tr773:  sm->cs = 282; goto _again;
	tr320:  sm->cs = 283; goto _again;
	tr321:  sm->cs = 284; goto _again;
	tr328:  sm->cs = 285; goto _again;
	tr329:  sm->cs = 286; goto _again;
	tr331:  sm->cs = 287; goto _again;
	tr330:  sm->cs = 288; goto _again;
	tr333:  sm->cs = 289; goto _again;
	tr334:  sm->cs = 290; goto _again;
	tr322:  sm->cs = 291; goto _again;
	tr323:  sm->cs = 292; goto _again;
	tr337:  sm->cs = 293; goto _again;
	tr338:  sm->cs = 294; goto _again;
	tr339:  sm->cs = 295; goto _again;
	tr340:  sm->cs = 296; goto _again;
	tr324:  sm->cs = 297; goto _again;
	tr342:  sm->cs = 298; goto _again;
	tr346:  sm->cs = 299; goto _again;
	tr347:  sm->cs = 300; goto _again;
	tr348:  sm->cs = 301; goto _again;
	tr349:  sm->cs = 302; goto _again;
	tr350:  sm->cs = 303; goto _again;
	tr343:  sm->cs = 304; goto _again;
	tr352:  sm->cs = 305; goto _again;
	tr353:  sm->cs = 306; goto _again;
	tr354:  sm->cs = 307; goto _again;
	tr355:  sm->cs = 308; goto _again;
	tr356:  sm->cs = 309; goto _again;
	tr357:  sm->cs = 310; goto _again;
	tr344:  sm->cs = 311; goto _again;
	tr359:  sm->cs = 312; goto _again;
	tr360:  sm->cs = 313; goto _again;
	tr325:  sm->cs = 314; goto _again;
	tr363:  sm->cs = 315; goto _again;
	tr367:  sm->cs = 316; goto _again;
	tr368:  sm->cs = 317; goto _again;
	tr369:  sm->cs = 318; goto _again;
	tr364:  sm->cs = 319; goto _again;
	tr365:  sm->cs = 320; goto _again;
	tr366:  sm->cs = 321; goto _again;
	tr326:  sm->cs = 322; goto _again;
	tr774:  sm->cs = 323; goto _again;
	tr775:  sm->cs = 324; goto _again;
	tr376:  sm->cs = 325; goto _again;
	tr377:  sm->cs = 326; goto _again;
	tr379:  sm->cs = 327; goto _again;
	tr378:  sm->cs = 328; goto _again;
	tr381:  sm->cs = 329; goto _again;
	tr382:  sm->cs = 330; goto _again;
	tr383:  sm->cs = 331; goto _again;
	tr384:  sm->cs = 332; goto f7;
	tr398:  sm->cs = 333; goto _again;
	tr399:  sm->cs = 334; goto _again;
	tr400:  sm->cs = 335; goto _again;
	tr401:  sm->cs = 336; goto _again;
	tr403:  sm->cs = 337; goto _again;
	tr404:  sm->cs = 338; goto _again;
	tr385:  sm->cs = 339; goto f7;
	tr405:  sm->cs = 340; goto _again;
	tr406:  sm->cs = 341; goto _again;
	tr407:  sm->cs = 342; goto _again;
	tr409:  sm->cs = 343; goto _again;
	tr410:  sm->cs = 344; goto _again;
	tr386:  sm->cs = 345; goto f7;
	tr411:  sm->cs = 346; goto _again;
	tr413:  sm->cs = 347; goto _again;
	tr414:  sm->cs = 348; goto _again;
	tr415:  sm->cs = 349; goto _again;
	tr416:  sm->cs = 350; goto _again;
	tr417:  sm->cs = 351; goto _again;
	tr418:  sm->cs = 352; goto _again;
	tr412:  sm->cs = 353; goto _again;
	tr419:  sm->cs = 354; goto _again;
	tr420:  sm->cs = 355; goto _again;
	tr421:  sm->cs = 356; goto _again;
	tr422:  sm->cs = 357; goto _again;
	tr423:  sm->cs = 358; goto _again;
	tr387:  sm->cs = 359; goto f7;
	tr424:  sm->cs = 360; goto _again;
	tr425:  sm->cs = 361; goto _again;
	tr426:  sm->cs = 362; goto _again;
	tr427:  sm->cs = 363; goto _again;
	tr428:  sm->cs = 364; goto _again;
	tr388:  sm->cs = 365; goto f7;
	tr429:  sm->cs = 366; goto _again;
	tr430:  sm->cs = 367; goto _again;
	tr389:  sm->cs = 368; goto f7;
	tr431:  sm->cs = 369; goto _again;
	tr432:  sm->cs = 370; goto _again;
	tr390:  sm->cs = 371; goto f7;
	tr433:  sm->cs = 372; goto _again;
	tr434:  sm->cs = 373; goto _again;
	tr435:  sm->cs = 374; goto _again;
	tr436:  sm->cs = 375; goto _again;
	tr437:  sm->cs = 376; goto _again;
	tr391:  sm->cs = 377; goto f7;
	tr438:  sm->cs = 378; goto _again;
	tr392:  sm->cs = 378; goto f7;
	tr439:  sm->cs = 379; goto _again;
	tr440:  sm->cs = 380; goto _again;
	tr441:  sm->cs = 381; goto _again;
	tr442:  sm->cs = 382; goto _again;
	tr443:  sm->cs = 383; goto _again;
	tr393:  sm->cs = 384; goto f7;
	tr444:  sm->cs = 385; goto _again;
	tr446:  sm->cs = 386; goto _again;
	tr447:  sm->cs = 387; goto _again;
	tr448:  sm->cs = 388; goto _again;
	tr449:  sm->cs = 389; goto _again;
	tr450:  sm->cs = 390; goto _again;
	tr451:  sm->cs = 391; goto _again;
	tr445:  sm->cs = 392; goto _again;
	tr452:  sm->cs = 393; goto _again;
	tr453:  sm->cs = 394; goto _again;
	tr454:  sm->cs = 395; goto _again;
	tr455:  sm->cs = 396; goto _again;
	tr456:  sm->cs = 397; goto _again;
	tr394:  sm->cs = 398; goto f7;
	tr457:  sm->cs = 399; goto _again;
	tr458:  sm->cs = 400; goto _again;
	tr459:  sm->cs = 401; goto _again;
	tr460:  sm->cs = 402; goto _again;
	tr461:  sm->cs = 403; goto _again;
	tr395:  sm->cs = 404; goto f7;
	tr462:  sm->cs = 405; goto _again;
	tr463:  sm->cs = 406; goto _again;
	tr396:  sm->cs = 407; goto f7;
	tr464:  sm->cs = 408; goto _again;
	tr465:  sm->cs = 409; goto _again;
	tr397:  sm->cs = 410; goto f7;
	tr466:  sm->cs = 411; goto _again;
	tr467:  sm->cs = 412; goto _again;
	tr468:  sm->cs = 413; goto _again;
	tr469:  sm->cs = 414; goto _again;
	tr470:  sm->cs = 415; goto _again;
	tr776:  sm->cs = 416; goto _again;
	tr777:  sm->cs = 417; goto _again;
	tr472:  sm->cs = 418; goto _again;
	tr473:  sm->cs = 419; goto _again;
	tr474:  sm->cs = 420; goto _again;
	tr475:  sm->cs = 421; goto _again;
	tr476:  sm->cs = 422; goto _again;
	tr477:  sm->cs = 423; goto _again;
	tr778:  sm->cs = 424; goto _again;
	tr479:  sm->cs = 425; goto _again;
	tr480:  sm->cs = 426; goto _again;
	tr481:  sm->cs = 427; goto _again;
	tr482:  sm->cs = 428; goto _again;
	tr779:  sm->cs = 429; goto _again;
	tr484:  sm->cs = 430; goto _again;
	tr488:  sm->cs = 431; goto _again;
	tr489:  sm->cs = 432; goto _again;
	tr490:  sm->cs = 433; goto _again;
	tr491:  sm->cs = 434; goto _again;
	tr492:  sm->cs = 435; goto _again;
	tr493:  sm->cs = 436; goto _again;
	tr496:  sm->cs = 437; goto _again;
	tr497:  sm->cs = 438; goto _again;
	tr498:  sm->cs = 439; goto _again;
	tr499:  sm->cs = 440; goto _again;
	tr500:  sm->cs = 441; goto _again;
	tr501:  sm->cs = 442; goto _again;
	tr502:  sm->cs = 443; goto _again;
	tr503:  sm->cs = 444; goto _again;
	tr504:  sm->cs = 445; goto _again;
	tr507:  sm->cs = 446; goto _again;
	tr506:  sm->cs = 446; goto f7;
	tr494:  sm->cs = 447; goto _again;
	tr510:  sm->cs = 448; goto _again;
	tr509:  sm->cs = 448; goto f7;
	tr485:  sm->cs = 449; goto _again;
	tr512:  sm->cs = 450; goto _again;
	tr513:  sm->cs = 451; goto _again;
	tr514:  sm->cs = 452; goto _again;
	tr515:  sm->cs = 453; goto _again;
	tr516:  sm->cs = 454; goto _again;
	tr517:  sm->cs = 455; goto _again;
	tr486:  sm->cs = 456; goto _again;
	tr519:  sm->cs = 457; goto _again;
	tr520:  sm->cs = 458; goto _again;
	tr780:  sm->cs = 459; goto _again;
	tr523:  sm->cs = 460; goto _again;
	tr525:  sm->cs = 461; goto _again;
	tr526:  sm->cs = 462; goto _again;
	tr527:  sm->cs = 463; goto _again;
	tr524:  sm->cs = 464; goto _again;
	tr781:  sm->cs = 465; goto _again;
	tr782:  sm->cs = 466; goto _again;
	tr533:  sm->cs = 467; goto _again;
	tr531:  sm->cs = 467; goto f7;
	tr534:  sm->cs = 468; goto f4;
	tr535:  sm->cs = 469; goto f4;
	tr538:  sm->cs = 470; goto _again;
	tr537:  sm->cs = 470; goto f3;
	tr539:  sm->cs = 471; goto f5;
	tr541:  sm->cs = 472; goto _again;
	tr532:  sm->cs = 472; goto f7;
	tr787:  sm->cs = 473; goto _again;
	tr542:  sm->cs = 474; goto _again;
	tr543:  sm->cs = 475; goto _again;
	tr544:  sm->cs = 476; goto _again;
	tr546:  sm->cs = 477; goto _again;
	tr547:  sm->cs = 478; goto _again;
	tr548:  sm->cs = 479; goto _again;
	tr549:  sm->cs = 480; goto _again;
	tr550:  sm->cs = 481; goto _again;
	tr545:  sm->cs = 482; goto _again;
	tr789:  sm->cs = 483; goto _again;
	tr554:  sm->cs = 484; goto _again;
	tr552:  sm->cs = 484; goto f7;
	tr555:  sm->cs = 485; goto f4;
	tr558:  sm->cs = 486; goto _again;
	tr557:  sm->cs = 486; goto f3;
	tr559:  sm->cs = 487; goto f5;
	tr556:  sm->cs = 488; goto f4;
	tr562:  sm->cs = 489; goto _again;
	tr553:  sm->cs = 489; goto f7;
	tr798:  sm->cs = 490; goto _again;
	tr564:  sm->cs = 491; goto _again;
	tr565:  sm->cs = 492; goto _again;
	tr566:  sm->cs = 493; goto _again;
	tr567:  sm->cs = 494; goto _again;
	tr802:  sm->cs = 495; goto _again;
	tr570:  sm->cs = 496; goto _again;
	tr571:  sm->cs = 497; goto _again;
	tr572:  sm->cs = 498; goto _again;
	tr573:  sm->cs = 499; goto _again;
	tr574:  sm->cs = 500; goto _again;
	tr575:  sm->cs = 501; goto _again;
	tr576:  sm->cs = 502; goto _again;
	tr806:  sm->cs = 503; goto _again;
	tr579:  sm->cs = 504; goto _again;
	tr580:  sm->cs = 505; goto _again;
	tr584:  sm->cs = 506; goto _again;
	tr585:  sm->cs = 507; goto _again;
	tr586:  sm->cs = 508; goto _again;
	tr581:  sm->cs = 509; goto _again;
	tr588:  sm->cs = 510; goto _again;
	tr589:  sm->cs = 511; goto _again;
	tr590:  sm->cs = 512; goto _again;
	tr582:  sm->cs = 513; goto _again;
	tr592:  sm->cs = 514; goto _again;
	tr593:  sm->cs = 515; goto _again;
	tr594:  sm->cs = 516; goto _again;
	tr583:  sm->cs = 517; goto _again;
	tr807:  sm->cs = 518; goto _again;
	tr597:  sm->cs = 519; goto _again;
	tr601:  sm->cs = 520; goto _again;
	tr602:  sm->cs = 521; goto _again;
	tr603:  sm->cs = 522; goto _again;
	tr598:  sm->cs = 523; goto _again;
	tr599:  sm->cs = 524; goto _again;
	tr606:  sm->cs = 525; goto _again;
	tr608:  sm->cs = 526; goto _again;
	tr609:  sm->cs = 527; goto _again;
	tr600:  sm->cs = 528; goto _again;
	tr812:  sm->cs = 529; goto _again;
	tr617:  sm->cs = 530; goto f4;
	tr618:  sm->cs = 531; goto _again;
	tr0:  sm->cs = 532; goto f0;
	tr2:  sm->cs = 532; goto f2;
	tr20:  sm->cs = 532; goto f6;
	tr75:  sm->cs = 532; goto f8;
	tr76:  sm->cs = 532; goto f9;
	tr619:  sm->cs = 532; goto f89;
	tr626:  sm->cs = 532; goto f92;
	tr628:  sm->cs = 532; goto f93;
	tr629:  sm->cs = 532; goto f94;
	tr632:  sm->cs = 532; goto f95;
	tr634:  sm->cs = 532; goto f96;
	tr642:  sm->cs = 532; goto f97;
	tr643:  sm->cs = 532; goto f98;
	tr644:  sm->cs = 532; goto f99;
	tr645:  sm->cs = 532; goto f100;
	tr647:  sm->cs = 532; goto f101;
	tr648:  sm->cs = 532; goto f102;
	tr650:  sm->cs = 532; goto f103;
	tr651:  sm->cs = 532; goto f104;
	tr652:  sm->cs = 532; goto f105;
	tr1:  sm->cs = 533; goto f1;
	tr620:  sm->cs = 533; goto f90;
	tr621:  sm->cs = 534; goto _again;
	tr622:  sm->cs = 535; goto f32;
	tr630:  sm->cs = 536; goto _again;
	tr3:  sm->cs = 536; goto f3;
	tr4:  sm->cs = 537; goto f3;
	tr623:  sm->cs = 538; goto f91;
	tr633:  sm->cs = 539; goto _again;
	tr11:  sm->cs = 539; goto f5;
	tr635:  sm->cs = 540; goto _again;
	tr8:  sm->cs = 540; goto f4;
	tr624:  sm->cs = 541; goto f91;
	tr24:  sm->cs = 542; goto _again;
	tr31:  sm->cs = 543; goto _again;
	tr36:  sm->cs = 544; goto _again;
	tr646:  sm->cs = 545; goto _again;
	tr59:  sm->cs = 545; goto f4;
	tr56:  sm->cs = 546; goto _again;
	tr649:  sm->cs = 547; goto _again;
	tr62:  sm->cs = 547; goto f4;
	tr46:  sm->cs = 548; goto _again;
	tr69:  sm->cs = 549; goto _again;
	tr625:  sm->cs = 550; goto _again;
	tr77:  sm->cs = 551; goto f10;
	tr82:  sm->cs = 551; goto f11;
	tr83:  sm->cs = 551; goto f12;
	tr85:  sm->cs = 551; goto f13;
	tr88:  sm->cs = 551; goto f14;
	tr89:  sm->cs = 551; goto f15;
	tr90:  sm->cs = 551; goto f16;
	tr91:  sm->cs = 551; goto f17;
	tr92:  sm->cs = 551; goto f18;
	tr94:  sm->cs = 551; goto f19;
	tr97:  sm->cs = 551; goto f20;
	tr98:  sm->cs = 551; goto f21;
	tr99:  sm->cs = 551; goto f22;
	tr653:  sm->cs = 551; goto f106;
	tr655:  sm->cs = 551; goto f107;
	tr654:  sm->cs = 552; goto f91;
	tr100:  sm->cs = 553; goto f23;
	tr102:  sm->cs = 553; goto f25;
	tr107:  sm->cs = 553; goto f26;
	tr124:  sm->cs = 553; goto f28;
	tr147:  sm->cs = 553; goto f29;
	tr159:  sm->cs = 553; goto f30;
	tr304:  sm->cs = 553; goto f33;
	tr319:  sm->cs = 553; goto f34;
	tr327:  sm->cs = 553; goto f35;
	tr335:  sm->cs = 553; goto f36;
	tr336:  sm->cs = 553; goto f37;
	tr345:  sm->cs = 553; goto f38;
	tr351:  sm->cs = 553; goto f39;
	tr358:  sm->cs = 553; goto f40;
	tr361:  sm->cs = 553; goto f41;
	tr362:  sm->cs = 553; goto f42;
	tr371:  sm->cs = 553; goto f43;
	tr372:  sm->cs = 553; goto f44;
	tr373:  sm->cs = 553; goto f45;
	tr374:  sm->cs = 553; goto f46;
	tr375:  sm->cs = 553; goto f47;
	tr380:  sm->cs = 553; goto f48;
	tr402:  sm->cs = 553; goto f49;
	tr408:  sm->cs = 553; goto f50;
	tr471:  sm->cs = 553; goto f51;
	tr478:  sm->cs = 553; goto f52;
	tr483:  sm->cs = 553; goto f53;
	tr487:  sm->cs = 553; goto f54;
	tr495:  sm->cs = 553; goto f55;
	tr505:  sm->cs = 553; goto f56;
	tr508:  sm->cs = 553; goto f57;
	tr511:  sm->cs = 553; goto f58;
	tr518:  sm->cs = 553; goto f59;
	tr521:  sm->cs = 553; goto f60;
	tr522:  sm->cs = 553; goto f61;
	tr528:  sm->cs = 553; goto f62;
	tr529:  sm->cs = 553; goto f63;
	tr530:  sm->cs = 553; goto f64;
	tr536:  sm->cs = 553; goto f65;
	tr540:  sm->cs = 553; goto f66;
	tr560:  sm->cs = 553; goto f68;
	tr561:  sm->cs = 553; goto f69;
	tr661:  sm->cs = 553; goto f108;
	tr682:  sm->cs = 553; goto f111;
	tr685:  sm->cs = 553; goto f112;
	tr688:  sm->cs = 553; goto f113;
	tr689:  sm->cs = 553; goto f114;
	tr691:  sm->cs = 553; goto f115;
	tr692:  sm->cs = 553; goto f116;
	tr694:  sm->cs = 553; goto f117;
	tr701:  sm->cs = 553; goto f119;
	tr703:  sm->cs = 553; goto f120;
	tr707:  sm->cs = 553; goto f122;
	tr709:  sm->cs = 553; goto f123;
	tr714:  sm->cs = 553; goto f124;
	tr716:  sm->cs = 553; goto f125;
	tr718:  sm->cs = 553; goto f126;
	tr721:  sm->cs = 553; goto f127;
	tr725:  sm->cs = 553; goto f128;
	tr727:  sm->cs = 553; goto f129;
	tr730:  sm->cs = 553; goto f130;
	tr733:  sm->cs = 553; goto f131;
	tr736:  sm->cs = 553; goto f132;
	tr739:  sm->cs = 553; goto f133;
	tr741:  sm->cs = 553; goto f134;
	tr743:  sm->cs = 553; goto f135;
	tr746:  sm->cs = 553; goto f136;
	tr749:  sm->cs = 553; goto f137;
	tr755:  sm->cs = 553; goto f138;
	tr757:  sm->cs = 553; goto f139;
	tr759:  sm->cs = 553; goto f140;
	tr761:  sm->cs = 553; goto f141;
	tr764:  sm->cs = 553; goto f142;
	tr767:  sm->cs = 553; goto f143;
	tr770:  sm->cs = 553; goto f144;
	tr783:  sm->cs = 553; goto f145;
	tr784:  sm->cs = 553; goto f146;
	tr785:  sm->cs = 553; goto f147;
	tr786:  sm->cs = 553; goto f148;
	tr788:  sm->cs = 553; goto f149;
	tr662:  sm->cs = 554; goto f109;
	tr101:  sm->cs = 555; goto f24;
	tr690:  sm->cs = 556; goto _again;
	tr105:  sm->cs = 556; goto f3;
	tr106:  sm->cs = 557; goto f3;
	tr663:  sm->cs = 558; goto _again;
	tr664:  sm->cs = 559; goto f110;
	tr117:  sm->cs = 560; goto f27;
	tr665:  sm->cs = 561; goto f91;
	tr666:  sm->cs = 562; goto f110;
	tr160:  sm->cs = 563; goto f31;
	tr704:  sm->cs = 563; goto f121;
	tr700:  sm->cs = 564; goto f118;
	tr667:  sm->cs = 565; goto f91;
	tr708:  sm->cs = 566; goto _again;
	tr168:  sm->cs = 566; goto f7;
	tr710:  sm->cs = 567; goto _again;
	tr175:  sm->cs = 567; goto f7;
	tr668:  sm->cs = 568; goto f91;
	tr715:  sm->cs = 569; goto _again;
	tr179:  sm->cs = 569; goto f7;
	tr717:  sm->cs = 570; goto _again;
	tr184:  sm->cs = 570; goto f7;
	tr719:  sm->cs = 571; goto _again;
	tr188:  sm->cs = 571; goto f7;
	tr669:  sm->cs = 572; goto f91;
	tr722:  sm->cs = 573; goto _again;
	tr196:  sm->cs = 573; goto f7;
	tr670:  sm->cs = 574; goto f91;
	tr726:  sm->cs = 575; goto _again;
	tr201:  sm->cs = 575; goto f7;
	tr728:  sm->cs = 576; goto _again;
	tr207:  sm->cs = 576; goto f7;
	tr671:  sm->cs = 577; goto f91;
	tr731:  sm->cs = 578; goto _again;
	tr219:  sm->cs = 578; goto f7;
	tr672:  sm->cs = 579; goto f91;
	tr734:  sm->cs = 580; goto _again;
	tr230:  sm->cs = 580; goto f7;
	tr673:  sm->cs = 581; goto f91;
	tr737:  sm->cs = 582; goto _again;
	tr235:  sm->cs = 582; goto f7;
	tr674:  sm->cs = 583; goto f91;
	tr740:  sm->cs = 584; goto _again;
	tr241:  sm->cs = 584; goto f7;
	tr742:  sm->cs = 585; goto _again;
	tr246:  sm->cs = 585; goto f7;
	tr744:  sm->cs = 586; goto _again;
	tr255:  sm->cs = 586; goto f7;
	tr675:  sm->cs = 587; goto f91;
	tr747:  sm->cs = 588; goto _again;
	tr262:  sm->cs = 588; goto f7;
	tr676:  sm->cs = 589; goto f91;
	tr750:  sm->cs = 590; goto _again;
	tr266:  sm->cs = 590; goto f7;
	tr677:  sm->cs = 591; goto f91;
	tr756:  sm->cs = 592; goto _again;
	tr277:  sm->cs = 592; goto f7;
	tr758:  sm->cs = 593; goto _again;
	tr290:  sm->cs = 593; goto f7;
	tr760:  sm->cs = 594; goto _again;
	tr297:  sm->cs = 594; goto f7;
	tr303:  sm->cs = 595; goto f32;
	tr763:  sm->cs = 595; goto f91;
	tr765:  sm->cs = 596; goto _again;
	tr306:  sm->cs = 596; goto f3;
	tr678:  sm->cs = 597; goto f91;
	tr768:  sm->cs = 598; goto _again;
	tr311:  sm->cs = 598; goto f7;
	tr679:  sm->cs = 599; goto f91;
	tr771:  sm->cs = 600; goto _again;
	tr316:  sm->cs = 600; goto f7;
	tr680:  sm->cs = 601; goto f91;
	tr332:  sm->cs = 602; goto _again;
	tr341:  sm->cs = 603; goto _again;
	tr370:  sm->cs = 604; goto _again;
	tr681:  sm->cs = 605; goto _again;
	tr683:  sm->cs = 606; goto f110;
	tr551:  sm->cs = 607; goto f67;
	tr684:  sm->cs = 608; goto f91;
	tr790:  sm->cs = 609; goto f150;
	tr792:  sm->cs = 609; goto f151;
	tr793:  sm->cs = 609; goto f152;
	tr794:  sm->cs = 609; goto f153;
	tr791:  sm->cs = 610; goto _again;
	tr563:  sm->cs = 611; goto f70;
	tr568:  sm->cs = 611; goto f71;
	tr795:  sm->cs = 611; goto f154;
	tr797:  sm->cs = 611; goto f155;
	tr796:  sm->cs = 612; goto f91;
	tr569:  sm->cs = 613; goto f72;
	tr577:  sm->cs = 613; goto f73;
	tr799:  sm->cs = 613; goto f156;
	tr801:  sm->cs = 613; goto f157;
	tr800:  sm->cs = 614; goto f91;
	tr578:  sm->cs = 615; goto f74;
	tr587:  sm->cs = 615; goto f75;
	tr591:  sm->cs = 615; goto f76;
	tr595:  sm->cs = 615; goto f77;
	tr596:  sm->cs = 615; goto f78;
	tr604:  sm->cs = 615; goto f79;
	tr605:  sm->cs = 615; goto f80;
	tr607:  sm->cs = 615; goto f81;
	tr610:  sm->cs = 615; goto f82;
	tr611:  sm->cs = 615; goto f83;
	tr803:  sm->cs = 615; goto f158;
	tr805:  sm->cs = 615; goto f159;
	tr804:  sm->cs = 616; goto f91;
	tr612:  sm->cs = 617; goto f84;
	tr614:  sm->cs = 617; goto f86;
	tr808:  sm->cs = 617; goto f160;
	tr813:  sm->cs = 617; goto f162;
	tr814:  sm->cs = 617; goto f163;
	tr613:  sm->cs = 618; goto f85;
	tr809:  sm->cs = 618; goto f161;
	tr810:  sm->cs = 619; goto _again;
	tr811:  sm->cs = 620; goto f32;
	tr815:  sm->cs = 621; goto _again;
	tr615:  sm->cs = 621; goto f3;
	tr616:  sm->cs = 622; goto f3;

f7:
#line 79 "ext/dtext/dtext.rl"
	{
  sm->a1 = sm->p;
}
	goto _again;
f4:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
	goto _again;
f3:
#line 87 "ext/dtext/dtext.rl"
	{
  sm->b1 = sm->p;
}
	goto _again;
f5:
#line 91 "ext/dtext/dtext.rl"
	{
  sm->b2 = sm->p;
}
	goto _again;
f91:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto _again;
f17:
#line 171 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(sm,  INLINE_B, "<strong>"); }}
	goto _again;
f11:
#line 172 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(sm, INLINE_B, "</strong>"); }}
	goto _again;
f18:
#line 173 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(sm,  INLINE_I, "<em>"); }}
	goto _again;
f12:
#line 174 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(sm, INLINE_I, "</em>"); }}
	goto _again;
f19:
#line 175 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(sm,  INLINE_S, "<s>"); }}
	goto _again;
f13:
#line 176 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(sm, INLINE_S, "</s>"); }}
	goto _again;
f22:
#line 177 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(sm,  INLINE_U, "<u>"); }}
	goto _again;
f16:
#line 178 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(sm, INLINE_U, "</u>"); }}
	goto _again;
f21:
#line 179 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(sm, INLINE_SUP, "<sup>"); }}
	goto _again;
f15:
#line 180 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(sm, INLINE_SUP, "</sup>"); }}
	goto _again;
f20:
#line 181 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(sm, INLINE_SUB, "<sub>"); }}
	goto _again;
f14:
#line 182 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(sm, INLINE_SUB, "</sub>"); }}
	goto _again;
f106:
#line 183 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ append_c_html_escaped(sm, (*( sm->p))); }}
	goto _again;
f107:
#line 183 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_c_html_escaped(sm, (*( sm->p))); }}
	goto _again;
f10:
#line 183 "ext/dtext/dtext.rl"
	{{( sm->p) = ((( sm->te)))-1;}{ append_c_html_escaped(sm, (*( sm->p))); }}
	goto _again;
f148:
#line 187 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    append(sm, true, "`");
  }}
	goto _again;
f111:
#line 191 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    append(sm, true, "<span class=\"inline-code\">");
    {
  size_t len = sm->stack->len;

  if (len > MAX_STACK_DEPTH) {
    g_set_error_literal(&sm->error, DTEXT_PARSE_ERROR, DTEXT_PARSE_ERROR_DEPTH_EXCEEDED, "too many nested elements");
    {( sm->p)++; goto _out; }
  }

  if (sm->top >= len) {
    g_debug("growing sm->stack %zi\n", len + 16);
    sm->stack = g_array_set_size(sm->stack, len + 16);
  }
{( ((int *)sm->stack->data))[( sm->top)++] =  sm->cs;  sm->cs = 609;goto _again;}}
  }}
	goto _again;
f69:
#line 310 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    append_post_search_link(sm, sm->a1, sm->a2 - sm->a1, sm->a1, sm->a2 - sm->a1);
  }}
	goto _again;
f68:
#line 314 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    append_post_search_link(sm, sm->a1, sm->a2 - sm->a1, sm->b1, sm->b2 - sm->b1);
  }}
	goto _again;
f65:
#line 318 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    append_wiki_link(sm, sm->a1, sm->a2 - sm->a1, sm->a1, sm->a2 - sm->a1);
  }}
	goto _again;
f66:
#line 322 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    append_wiki_link(sm, sm->a1, sm->a2 - sm->a1, sm->b1, sm->b2 - sm->b1);
  }}
	goto _again;
f28:
#line 340 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    if (!append_named_url(sm, sm->b1, sm->b2, sm->a1, sm->a2)) {
      {( sm->p)++; goto _out; }
    }
  }}
	goto _again;
f30:
#line 358 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    append_url(sm, sm->ts + 1, sm->te - 2, sm->ts + 1, sm->te - 2);
  }}
	goto _again;
f47:
#line 420 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(sm,  INLINE_B, "<strong>"); }}
	goto _again;
f35:
#line 421 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(sm, INLINE_B, "</strong>"); }}
	goto _again;
f51:
#line 422 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(sm,  INLINE_I, "<em>"); }}
	goto _again;
f37:
#line 423 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(sm, INLINE_I, "</em>"); }}
	goto _again;
f54:
#line 424 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(sm,  INLINE_S, "<s>"); }}
	goto _again;
f38:
#line 425 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(sm, INLINE_S, "</s>"); }}
	goto _again;
f64:
#line 426 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(sm,  INLINE_U, "<u>"); }}
	goto _again;
f46:
#line 427 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(sm, INLINE_U, "</u>"); }}
	goto _again;
f61:
#line 428 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(sm, INLINE_SUP, "<sup>"); }}
	goto _again;
f42:
#line 429 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(sm, INLINE_SUP, "</sup>"); }}
	goto _again;
f60:
#line 430 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(sm, INLINE_SUB, "<sub>"); }}
	goto _again;
f41:
#line 431 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(sm, INLINE_SUB, "</sub>"); }}
	goto _again;
f63:
#line 433 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_open_inline(sm, INLINE_TN, "<span class=\"tn\">");
  }}
	goto _again;
f45:
#line 437 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_close_before_block(sm);

    if (dstack_check(sm, INLINE_TN)) {
      dstack_close_inline(sm, INLINE_TN, "</span>");
    } else if (dstack_close_block(sm, BLOCK_TN, "</p>")) {
      { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
    }
  }}
	goto _again;
f36:
#line 470 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    if(!sm->allow_color)
      { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
    dstack_close_inline(sm, INLINE_COLOR, "</span>");
  }}
	goto _again;
f59:
#line 476 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_open_inline(sm, INLINE_SPOILER, "<span class=\"spoiler\">");
  }}
	goto _again;
f40:
#line 480 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    g_debug("inline [/spoiler]");
    dstack_close_before_block(sm);

    if (dstack_check(sm, INLINE_SPOILER)) {
      dstack_close_inline(sm, INLINE_SPOILER, "</span>");
    } else if (dstack_close_block(sm, BLOCK_SPOILER, "</div>")) {
      { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
    }
  }}
	goto _again;
f52:
#line 491 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_open_inline(sm, INLINE_NODTEXT, "");
    {
  size_t len = sm->stack->len;

  if (len > MAX_STACK_DEPTH) {
    g_set_error_literal(&sm->error, DTEXT_PARSE_ERROR, DTEXT_PARSE_ERROR_DEPTH_EXCEEDED, "too many nested elements");
    {( sm->p)++; goto _out; }
  }

  if (sm->top >= len) {
    g_debug("growing sm->stack %zi\n", len + 16);
    sm->stack = g_array_set_size(sm->stack, len + 16);
  }
{( ((int *)sm->stack->data))[( sm->top)++] =  sm->cs;  sm->cs = 613;goto _again;}}
  }}
	goto _again;
f62:
#line 499 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_close_before_block(sm);
    {( sm->p) = (( sm->ts))-1;}
    { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
  }}
	goto _again;
f48:
#line 521 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_close_before_block(sm);
    {( sm->p) = (( sm->ts))-1;}
    { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
  }}
	goto _again;
f53:
#line 543 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    g_debug("inline [quote]");
    dstack_close_before_block(sm);
    {( sm->p) = (( sm->ts))-1;}
    { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
  }}
	goto _again;
f55:
#line 566 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    g_debug("inline [section]");
    dstack_rewind(sm);
    {( sm->p) = (((sm->p - 8)))-1;}
    { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
  }}
	goto _again;
f56:
#line 573 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    g_debug("inline expanded [section]");
    dstack_rewind(sm);
    {( sm->p) = (((sm->p - 17)))-1;}
    { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
  }}
	goto _again;
f39:
#line 580 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_close_before_block(sm);

    if (dstack_close_block(sm, BLOCK_SECTION, "</div></div>")) {
      { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
    }
  }}
	goto _again;
f44:
#line 600 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    if (dstack_close_block(sm, BLOCK_TH, "</th>")) {
      { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
    }
  }}
	goto _again;
f43:
#line 606 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    if (dstack_close_block(sm, BLOCK_TD, "</td>")) {
      { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
    }
  }}
	goto _again;
f108:
#line 642 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    g_debug("inline char: %c", (*( sm->p)));
    append_c_html_escaped(sm, (*( sm->p)));
  }}
	goto _again;
f117:
#line 326 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    const char* match_end = sm->b2;
    const char* url_start = sm->b1;
    const char* url_end = find_boundary_c(match_end);

    if (!append_named_url(sm, url_start, url_end, sm->a1, sm->a2)) {
      {( sm->p)++; goto _out; }
    }

    if (url_end < match_end) {
      append_segment_html_escaped(sm, url_end + 1, match_end);
    }
  }}
	goto _again;
f149:
#line 346 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    const char* match_end = sm->te - 1;
    const char* url_start = sm->ts;
    const char* url_end = find_boundary_c(match_end);

    append_url(sm, url_start, url_end, url_start, url_end);

    if (url_end < match_end) {
      append_segment_html_escaped(sm, url_end + 1, match_end);
    }
  }}
	goto _again;
f147:
#line 505 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("inline [/table]");
    dstack_close_before_block(sm);

    if (dstack_check(sm, BLOCK_LI)) {
      dstack_close_list(sm);
    }

    if (dstack_check(sm, BLOCK_TABLE)) {
      dstack_rewind(sm);
      { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
    } else {
      append_block(sm, "[/table]");
    }
  }}
	goto _again;
f145:
#line 527 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("inline [/code]");
    dstack_close_before_block(sm);

    if (dstack_check(sm, BLOCK_LI)) {
      dstack_close_list(sm);
    }

    if (dstack_check(sm, BLOCK_CODE)) {
      dstack_rewind(sm);
      { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
    } else {
      append_block(sm, "[/code]");
    }
  }}
	goto _again;
f146:
#line 550 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("inline [/quote]");
    dstack_close_before_block(sm);

    if (dstack_check(sm, BLOCK_LI)) {
      dstack_close_list(sm);
    }

    if (dstack_check(sm, BLOCK_QUOTE)) {
      dstack_rewind(sm);
      { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
    } else {
      append_block(sm, "[/quote]");
    }
  }}
	goto _again;
f113:
#line 612 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("inline newline2");
    g_debug("  return");

    if (sm->list_mode) {
      dstack_close_list(sm);
    }

    {( sm->p) = (( sm->ts))-1;}
    { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
  }}
	goto _again;
f112:
#line 624 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("inline newline");

    if (sm->header_mode) {
      sm->header_mode = false;
      dstack_rewind(sm);
      { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
    } else if (sm->f_strip) {
      append_c(sm, ' ');
    } else {
      append(sm, true, "<br>");
    }
  }}
	goto _again;
f115:
#line 638 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_c(sm, ' ');
  }}
	goto _again;
f116:
#line 642 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("inline char: %c", (*( sm->p)));
    append_c_html_escaped(sm, (*( sm->p)));
  }}
	goto _again;
f33:
#line 242 "ext/dtext/dtext.rl"
	{{( sm->p) = ((( sm->te)))-1;}{
    append_link(sm, "topic #", "<a class=\"dtext-link dtext-id-link dtext-forum-topic-id-link\" href=\"/forum_topics/");
  }}
	goto _again;
f25:
#line 624 "ext/dtext/dtext.rl"
	{{( sm->p) = ((( sm->te)))-1;}{
    g_debug("inline newline");

    if (sm->header_mode) {
      sm->header_mode = false;
      dstack_rewind(sm);
      { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
    } else if (sm->f_strip) {
      append_c(sm, ' ');
    } else {
      append(sm, true, "<br>");
    }
  }}
	goto _again;
f26:
#line 642 "ext/dtext/dtext.rl"
	{{( sm->p) = ((( sm->te)))-1;}{
    g_debug("inline char: %c", (*( sm->p)));
    append_c_html_escaped(sm, (*( sm->p)));
  }}
	goto _again;
f23:
#line 1 "NONE"
	{	switch( ( sm->act) ) {
	case 44:
	{{( sm->p) = ((( sm->te)))-1;}
    const char* match_end = sm->b2;
    const char* url_start = sm->b1;
    const char* url_end = find_boundary_c(match_end);

    if (!append_named_url(sm, url_start, url_end, sm->a1, sm->a2)) {
      {( sm->p)++; goto _out; }
    }

    if (url_end < match_end) {
      append_segment_html_escaped(sm, url_end + 1, match_end);
    }
  }
	break;
	case 46:
	{{( sm->p) = ((( sm->te)))-1;}
    const char* match_end = sm->te - 1;
    const char* url_start = sm->ts;
    const char* url_end = find_boundary_c(match_end);

    append_url(sm, url_start, url_end, url_start, url_end);

    if (url_end < match_end) {
      append_segment_html_escaped(sm, url_end + 1, match_end);
    }
  }
	break;
	case 48:
	{{( sm->p) = ((( sm->te)))-1;}
    append_segment_html_escaped(sm, sm->ts, sm->te - 1);
  }
	break;
	case 49:
	{{( sm->p) = ((( sm->te)))-1;}
    if (!sm->f_mentions || (sm->a1 > sm->pb && sm->a1 - 1 > sm->pb && sm->a1[-2] != ' ' && sm->a1[-2] != '\r' && sm->a1[-2] != '\n')) {
      // handle emails
      append_c(sm, '@');
      {( sm->p) = (( sm->a1))-1;}
    } else {
      const char* match_end = sm->a2 - 1;
      const char* name_start = sm->a1;
      const char* name_end = find_boundary_c(match_end);

      append(sm, true, "<a rel=\"nofollow\" href=\"/users?name=");
      append_segment_uri_escaped(sm, name_start, name_end);
      append(sm, true, "\">");
      append_c(sm, '@');
      append_segment_html_escaped(sm, name_start, name_end);
      append(sm, true, "</a>");

      if (name_end < match_end) {
        append_segment_html_escaped(sm, name_end + 1, match_end);
      }
    }
  }
	break;
	case 85:
	{{( sm->p) = ((( sm->te)))-1;}
    g_debug("inline newline2");
    g_debug("  return");

    if (sm->list_mode) {
      dstack_close_list(sm);
    }

    {( sm->p) = (( sm->ts))-1;}
    { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
  }
	break;
	case 86:
	{{( sm->p) = ((( sm->te)))-1;}
    g_debug("inline newline");

    if (sm->header_mode) {
      sm->header_mode = false;
      dstack_rewind(sm);
      { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
    } else if (sm->f_strip) {
      append_c(sm, ' ');
    } else {
      append(sm, true, "<br>");
    }
  }
	break;
	case 88:
	{{( sm->p) = ((( sm->te)))-1;}
    g_debug("inline char: %c", (*( sm->p)));
    append_c_html_escaped(sm, (*( sm->p)));
  }
	break;
	}
	}
	goto _again;
f153:
#line 649 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    append(sm, true, "`");
  }}
	goto _again;
f151:
#line 653 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    append(sm, true, "</span>");
    { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
  }}
	goto _again;
f150:
#line 658 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    append_c_html_escaped(sm, (*( sm->p)));
  }}
	goto _again;
f152:
#line 658 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_c_html_escaped(sm, (*( sm->p)));
  }}
	goto _again;
f71:
#line 664 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    if (dstack_check(sm, BLOCK_CODE)) {
      dstack_rewind(sm);
    } else {
      append(sm, true, "[/code]");
    }
    { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
  }}
	goto _again;
f154:
#line 673 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    append_c_html_escaped(sm, (*( sm->p)));
  }}
	goto _again;
f155:
#line 673 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_c_html_escaped(sm, (*( sm->p)));
  }}
	goto _again;
f70:
#line 673 "ext/dtext/dtext.rl"
	{{( sm->p) = ((( sm->te)))-1;}{
    append_c_html_escaped(sm, (*( sm->p)));
  }}
	goto _again;
f73:
#line 679 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    if (dstack_check2(sm, BLOCK_NODTEXT)) {
      g_debug("block dstack check");
      dstack_pop(sm);
      dstack_pop(sm);
      append_block(sm, "</p>");
      { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
    } else if (dstack_check(sm, INLINE_NODTEXT)) {
      g_debug("inline dstack check");
      dstack_pop(sm);
      { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
    } else {
      g_debug("else dstack check");
      append(sm, true, "[/nodtext]");
    }
  }}
	goto _again;
f156:
#line 696 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    append_c_html_escaped(sm, (*( sm->p)));
  }}
	goto _again;
f157:
#line 696 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_c_html_escaped(sm, (*( sm->p)));
  }}
	goto _again;
f72:
#line 696 "ext/dtext/dtext.rl"
	{{( sm->p) = ((( sm->te)))-1;}{
    append_c_html_escaped(sm, (*( sm->p)));
  }}
	goto _again;
f82:
#line 702 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_open_block(sm, BLOCK_THEAD, "<thead>");
  }}
	goto _again;
f77:
#line 706 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_close_block(sm, BLOCK_THEAD, "</thead>");
  }}
	goto _again;
f79:
#line 710 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_open_block(sm, BLOCK_TBODY, "<tbody>");
  }}
	goto _again;
f76:
#line 714 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_close_block(sm, BLOCK_TBODY, "</tbody>");
  }}
	goto _again;
f81:
#line 718 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_open_block(sm, BLOCK_TH, "<th>");
    {
  size_t len = sm->stack->len;

  if (len > MAX_STACK_DEPTH) {
    g_set_error_literal(&sm->error, DTEXT_PARSE_ERROR, DTEXT_PARSE_ERROR_DEPTH_EXCEEDED, "too many nested elements");
    {( sm->p)++; goto _out; }
  }

  if (sm->top >= len) {
    g_debug("growing sm->stack %zi\n", len + 16);
    sm->stack = g_array_set_size(sm->stack, len + 16);
  }
{( ((int *)sm->stack->data))[( sm->top)++] =  sm->cs;  sm->cs = 553;goto _again;}}
  }}
	goto _again;
f83:
#line 723 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_open_block(sm, BLOCK_TR, "<tr>");
  }}
	goto _again;
f78:
#line 727 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_close_block(sm, BLOCK_TR, "</tr>");
  }}
	goto _again;
f80:
#line 731 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_open_block(sm, BLOCK_TD, "<td>");
    {
  size_t len = sm->stack->len;

  if (len > MAX_STACK_DEPTH) {
    g_set_error_literal(&sm->error, DTEXT_PARSE_ERROR, DTEXT_PARSE_ERROR_DEPTH_EXCEEDED, "too many nested elements");
    {( sm->p)++; goto _out; }
  }

  if (sm->top >= len) {
    g_debug("growing sm->stack %zi\n", len + 16);
    sm->stack = g_array_set_size(sm->stack, len + 16);
  }
{( ((int *)sm->stack->data))[( sm->top)++] =  sm->cs;  sm->cs = 553;goto _again;}}
  }}
	goto _again;
f75:
#line 736 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    if (dstack_close_block(sm, BLOCK_TABLE, "</table>")) {
      { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
    }
  }}
	goto _again;
f158:
#line 742 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;}
	goto _again;
f159:
#line 742 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;}
	goto _again;
f74:
#line 742 "ext/dtext/dtext.rl"
	{{( sm->p) = ((( sm->te)))-1;}}
	goto _again;
f160:
#line 787 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_rewind(sm);
    ( sm->p)--;
    { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
  }}
	goto _again;
f162:
#line 787 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    dstack_rewind(sm);
    ( sm->p)--;
    { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
  }}
	goto _again;
f86:
#line 787 "ext/dtext/dtext.rl"
	{{( sm->p) = ((( sm->te)))-1;}{
    dstack_rewind(sm);
    ( sm->p)--;
    { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
  }}
	goto _again;
f84:
#line 1 "NONE"
	{	switch( ( sm->act) ) {
	case 107:
	{{( sm->p) = ((( sm->te)))-1;}
    dstack_close_list(sm);
    {( sm->p) = (( sm->ts))-1;}
    { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
  }
	break;
	default:
	{{( sm->p) = ((( sm->te)))-1;}}
	break;
	}
	}
	goto _again;
f105:
#line 795 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    append(sm, true, "`");
  }}
	goto _again;
f92:
#line 799 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    append(sm, true, "<span class=\"inline-code\">");
    {
  size_t len = sm->stack->len;

  if (len > MAX_STACK_DEPTH) {
    g_set_error_literal(&sm->error, DTEXT_PARSE_ERROR, DTEXT_PARSE_ERROR_DEPTH_EXCEEDED, "too many nested elements");
    {( sm->p)++; goto _out; }
  }

  if (sm->top >= len) {
    g_debug("growing sm->stack %zi\n", len + 16);
    sm->stack = g_array_set_size(sm->stack, len + 16);
  }
{( ((int *)sm->stack->data))[( sm->top)++] =  sm->cs;  sm->cs = 609;goto _again;}}
  }}
	goto _again;
f6:
#line 926 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    g_debug("block [/spoiler]");
    dstack_close_before_block(sm);
    if (dstack_check(sm, BLOCK_SPOILER)) {
      g_debug("  rewind");
      dstack_rewind(sm);
    }
  }}
	goto _again;
f8:
#line 988 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_close_before_block(sm);
    dstack_open_block(sm, BLOCK_TABLE, "<table class=\"striped\">");
    {
  size_t len = sm->stack->len;

  if (len > MAX_STACK_DEPTH) {
    g_set_error_literal(&sm->error, DTEXT_PARSE_ERROR, DTEXT_PARSE_ERROR_DEPTH_EXCEEDED, "too many nested elements");
    {( sm->p)++; goto _out; }
  }

  if (sm->top >= len) {
    g_debug("growing sm->stack %zi\n", len + 16);
    sm->stack = g_array_set_size(sm->stack, len + 16);
  }
{( ((int *)sm->stack->data))[( sm->top)++] =  sm->cs;  sm->cs = 615;goto _again;}}
  }}
	goto _again;
f9:
#line 994 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_open_block(sm, BLOCK_TN, "<p class=\"tn\">");
    {
  size_t len = sm->stack->len;

  if (len > MAX_STACK_DEPTH) {
    g_set_error_literal(&sm->error, DTEXT_PARSE_ERROR, DTEXT_PARSE_ERROR_DEPTH_EXCEEDED, "too many nested elements");
    {( sm->p)++; goto _out; }
  }

  if (sm->top >= len) {
    g_debug("growing sm->stack %zi\n", len + 16);
    sm->stack = g_array_set_size(sm->stack, len + 16);
  }
{( ((int *)sm->stack->data))[( sm->top)++] =  sm->cs;  sm->cs = 553;goto _again;}}
  }}
	goto _again;
f89:
#line 1026 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    g_debug("block char: %c", (*( sm->p)));
    ( sm->p)--;

    if (g_queue_is_empty(sm->dstack) || dstack_check(sm, BLOCK_QUOTE) || dstack_check(sm, BLOCK_SPOILER) || dstack_check(sm, BLOCK_SECTION)) {
      dstack_open_block(sm, BLOCK_P, "<p>");
    }

    {
  size_t len = sm->stack->len;

  if (len > MAX_STACK_DEPTH) {
    g_set_error_literal(&sm->error, DTEXT_PARSE_ERROR, DTEXT_PARSE_ERROR_DEPTH_EXCEEDED, "too many nested elements");
    {( sm->p)++; goto _out; }
  }

  if (sm->top >= len) {
    g_debug("growing sm->stack %zi\n", len + 16);
    sm->stack = g_array_set_size(sm->stack, len + 16);
  }
{( ((int *)sm->stack->data))[( sm->top)++] =  sm->cs;  sm->cs = 553;goto _again;}}
  }}
	goto _again;
f95:
#line 804 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    char header = *sm->a1;
    g_autoptr(GString) id_name = g_string_new_len(sm->b1, sm->b2 - sm->b1);
    id_name = g_string_prepend(id_name, "dtext-");

    if (sm->f_inline) {
      header = '6';
    }

    if (sm->f_strip) {
      dstack_push(sm, BLOCK_STRIP);
    }

    if (!sm->f_strip) {
      switch (header) {
        case '1':
          dstack_push(sm, BLOCK_H1);
          append_block(sm, "<h1 id=\"");
          append_block(sm, id_name->str);
          append_block(sm, "\">");
          break;

        case '2':
          dstack_push(sm, BLOCK_H2);
          append_block(sm, "<h2 id=\"");
          append_block(sm, id_name->str);
          append_block(sm, "\">");
          break;

        case '3':
          dstack_push(sm, BLOCK_H3);
          append_block(sm, "<h3 id=\"");
          append_block(sm, id_name->str);
          append_block(sm, "\">");
          break;

        case '4':
          dstack_push(sm, BLOCK_H4);
          append_block(sm, "<h4 id=\"");
          append_block(sm, id_name->str);
          append_block(sm, "\">");
          break;

        case '5':
          dstack_push(sm, BLOCK_H5);
          append_block(sm, "<h5 id=\"");
          append_block(sm, id_name->str);
          append_block(sm, "\">");
          break;

        case '6':
          dstack_push(sm, BLOCK_H6);
          append_block(sm, "<h6 id=\"");
          append_block(sm, id_name->str);
          append_block(sm, "\">");
          break;
      }
    }

    sm->header_mode = true;
    {
  size_t len = sm->stack->len;

  if (len > MAX_STACK_DEPTH) {
    g_set_error_literal(&sm->error, DTEXT_PARSE_ERROR, DTEXT_PARSE_ERROR_DEPTH_EXCEEDED, "too many nested elements");
    {( sm->p)++; goto _out; }
  }

  if (sm->top >= len) {
    g_debug("growing sm->stack %zi\n", len + 16);
    sm->stack = g_array_set_size(sm->stack, len + 16);
  }
{( ((int *)sm->stack->data))[( sm->top)++] =  sm->cs;  sm->cs = 553;goto _again;}}
  }}
	goto _again;
f96:
#line 867 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    char header = *sm->a1;

    if (sm->f_inline) {
      header = '6';
    }

    if (sm->f_strip) {
      dstack_push(sm, BLOCK_STRIP);
    }

    if (!sm->f_strip) {
      switch (header) {
        case '1':
          dstack_push(sm, BLOCK_H1);
          append_block(sm, "<h1>");
          break;

        case '2':
          dstack_push(sm, BLOCK_H2);
          append_block(sm, "<h2>");
          break;

        case '3':
          dstack_push(sm, BLOCK_H3);
          append_block(sm, "<h3>");
          break;

        case '4':
          dstack_push(sm, BLOCK_H4);
          append_block(sm, "<h4>");
          break;

        case '5':
          dstack_push(sm, BLOCK_H5);
          append_block(sm, "<h5>");
          break;

        case '6':
          dstack_push(sm, BLOCK_H6);
          append_block(sm, "<h6>");
          break;
      }
    }

    sm->header_mode = true;
    {
  size_t len = sm->stack->len;

  if (len > MAX_STACK_DEPTH) {
    g_set_error_literal(&sm->error, DTEXT_PARSE_ERROR, DTEXT_PARSE_ERROR_DEPTH_EXCEEDED, "too many nested elements");
    {( sm->p)++; goto _out; }
  }

  if (sm->top >= len) {
    g_debug("growing sm->stack %zi\n", len + 16);
    sm->stack = g_array_set_size(sm->stack, len + 16);
  }
{( ((int *)sm->stack->data))[( sm->top)++] =  sm->cs;  sm->cs = 553;goto _again;}}
  }}
	goto _again;
f99:
#line 916 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    dstack_close_before_block(sm);
    dstack_open_block(sm, BLOCK_QUOTE, "<blockquote>");
  }}
	goto _again;
f104:
#line 921 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    dstack_close_before_block(sm);
    dstack_open_block(sm, BLOCK_SPOILER, "<div class=\"spoiler\">");
  }}
	goto _again;
f97:
#line 935 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    dstack_close_before_block(sm);
    dstack_open_block(sm, BLOCK_CODE, "<pre>");
    {
  size_t len = sm->stack->len;

  if (len > MAX_STACK_DEPTH) {
    g_set_error_literal(&sm->error, DTEXT_PARSE_ERROR, DTEXT_PARSE_ERROR_DEPTH_EXCEEDED, "too many nested elements");
    {( sm->p)++; goto _out; }
  }

  if (sm->top >= len) {
    g_debug("growing sm->stack %zi\n", len + 16);
    sm->stack = g_array_set_size(sm->stack, len + 16);
  }
{( ((int *)sm->stack->data))[( sm->top)++] =  sm->cs;  sm->cs = 611;goto _again;}}
  }}
	goto _again;
f103:
#line 941 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    dstack_close_before_block(sm);
    const char* html = "<div class=\"expandable\"><div class=\"expandable-header\">"
                       "<span class=\"section-arrow\"></span></div>"
                       "<div class=\"expandable-content\">";
    dstack_open_block(sm, BLOCK_SECTION, html);
  }}
	goto _again;
f101:
#line 949 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    dstack_close_before_block(sm);
    const char* html = "<div class=\"expandable expanded\"><div class=\"expandable-header\">"
                       "<span class=\"section-arrow expanded\"></span></div>"
                       "<div class=\"expandable-content\">";
    dstack_open_block(sm, BLOCK_SECTION, html);
  }}
	goto _again;
f102:
#line 957 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("block [section=]");
    dstack_close_before_block(sm);
    dstack_push(sm, BLOCK_SECTION);
    append_block(sm, "<div class=\"expandable\"><div class=\"expandable-header\"><span class=\"section-arrow\"></span>");
    append(sm, true, "<span>");
    append_segment_html_escaped(sm, sm->a1, sm->a2 - 1);
    append(sm, true, "</span>");
    append_block(sm, "</div>");
    append_block(sm, "<div class=\"expandable-content\">");
  }}
	goto _again;
f100:
#line 969 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("block expanded [section=]");
    dstack_close_before_block(sm);
    dstack_push(sm, BLOCK_SECTION);
    append_block(sm, "<div class=\"expandable expanded\"><div class=\"expandable-header\"><span class=\"section-arrow expanded\"></span>");
    append(sm, true, "<span>");
    append_segment_html_escaped(sm, sm->a1, sm->a2 - 1);
    append(sm, true, "</span>");
    append_block(sm, "</div>");
    append_block(sm, "<div class=\"expandable-content\">");
  }}
	goto _again;
f98:
#line 981 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    dstack_close_before_block(sm);
    dstack_open_block(sm, BLOCK_NODTEXT, "");
    dstack_open_block(sm, BLOCK_P, "<p>");
    {
  size_t len = sm->stack->len;

  if (len > MAX_STACK_DEPTH) {
    g_set_error_literal(&sm->error, DTEXT_PARSE_ERROR, DTEXT_PARSE_ERROR_DEPTH_EXCEEDED, "too many nested elements");
    {( sm->p)++; goto _out; }
  }

  if (sm->top >= len) {
    g_debug("growing sm->stack %zi\n", len + 16);
    sm->stack = g_array_set_size(sm->stack, len + 16);
  }
{( ((int *)sm->stack->data))[( sm->top)++] =  sm->cs;  sm->cs = 613;goto _again;}}
  }}
	goto _again;
f93:
#line 1026 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("block char: %c", (*( sm->p)));
    ( sm->p)--;

    if (g_queue_is_empty(sm->dstack) || dstack_check(sm, BLOCK_QUOTE) || dstack_check(sm, BLOCK_SPOILER) || dstack_check(sm, BLOCK_SECTION)) {
      dstack_open_block(sm, BLOCK_P, "<p>");
    }

    {
  size_t len = sm->stack->len;

  if (len > MAX_STACK_DEPTH) {
    g_set_error_literal(&sm->error, DTEXT_PARSE_ERROR, DTEXT_PARSE_ERROR_DEPTH_EXCEEDED, "too many nested elements");
    {( sm->p)++; goto _out; }
  }

  if (sm->top >= len) {
    g_debug("growing sm->stack %zi\n", len + 16);
    sm->stack = g_array_set_size(sm->stack, len + 16);
  }
{( ((int *)sm->stack->data))[( sm->top)++] =  sm->cs;  sm->cs = 553;goto _again;}}
  }}
	goto _again;
f2:
#line 1026 "ext/dtext/dtext.rl"
	{{( sm->p) = ((( sm->te)))-1;}{
    g_debug("block char: %c", (*( sm->p)));
    ( sm->p)--;

    if (g_queue_is_empty(sm->dstack) || dstack_check(sm, BLOCK_QUOTE) || dstack_check(sm, BLOCK_SPOILER) || dstack_check(sm, BLOCK_SECTION)) {
      dstack_open_block(sm, BLOCK_P, "<p>");
    }

    {
  size_t len = sm->stack->len;

  if (len > MAX_STACK_DEPTH) {
    g_set_error_literal(&sm->error, DTEXT_PARSE_ERROR, DTEXT_PARSE_ERROR_DEPTH_EXCEEDED, "too many nested elements");
    {( sm->p)++; goto _out; }
  }

  if (sm->top >= len) {
    g_debug("growing sm->stack %zi\n", len + 16);
    sm->stack = g_array_set_size(sm->stack, len + 16);
  }
{( ((int *)sm->stack->data))[( sm->top)++] =  sm->cs;  sm->cs = 553;goto _again;}}
  }}
	goto _again;
f0:
#line 1 "NONE"
	{	switch( ( sm->act) ) {
	case 126:
	{{( sm->p) = ((( sm->te)))-1;}
    g_debug("block newline2");

    if (sm->header_mode) {
      sm->header_mode = false;
      dstack_rewind(sm);
    } else if (sm->list_mode) {
      dstack_close_list(sm);
    } else {
      dstack_close_before_block(sm);
    }
  }
	break;
	case 127:
	{{( sm->p) = ((( sm->te)))-1;}
    g_debug("block newline");
  }
	break;
	}
	}
	goto _again;
f34:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 200 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    append(sm, true, "<a id=\"");
    g_autofree gchar* lowercased_tag = g_utf8_strdown(sm->a1, sm->a2-sm->a1);
    const size_t tag_len = sm->a2 - sm->a1;
    append_segment_uri_escaped(sm, lowercased_tag, lowercased_tag + tag_len -1);
    append(sm, true, "\"></a>");
  }}
	goto _again;
f29:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 390 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    if (sm->f_mentions) {
      append(sm, true, "<a rel=\"nofollow\" href=\"/users?name=");
      append_segment_uri_escaped(sm, sm->a1, sm->a2 - 1);
      append(sm, true, "\">");
      append_c(sm, '@');
      append_segment_html_escaped(sm, sm->a1, sm->a2 - 1);
      append(sm, true, "</a>");
    }
  }}
	goto _again;
f50:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 447 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    if(!sm->allow_color)
      { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
    dstack_push(sm, INLINE_COLOR);
    append(sm, true, "<span class=\"dtext-color-");
    append_segment_uri_escaped(sm, sm->a1, sm->a2-1);
    append(sm, true, "\">");
  }}
	goto _again;
f49:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 456 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    if(!sm->allow_color)
      { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
    dstack_push(sm, INLINE_COLOR);
    append(sm, true, "<span class=\"dtext-color\" style=\"color:");
    if(sm->a1[0] == '#') {
      append(sm, true, "#");
      append_segment_uri_escaped(sm, sm->a1 + 1, sm->a2-1);
    } else {
      append_segment_uri_escaped(sm, sm->a1, sm->a2-1);
    }
    append(sm, true, "\">");
  }}
	goto _again;
f58:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 588 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_rewind(sm);
    {( sm->p) = (((sm->p - 9 - (sm->a2 - sm->a1))))-1;}
    { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
  }}
	goto _again;
f57:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 594 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_rewind(sm);
    {( sm->p) = (((sm->p - 18 - (sm->a2 - sm->a1))))-1;}
    { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
  }}
	goto _again;
f134:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 196 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "post #", "<a class=\"dtext-link dtext-id-link dtext-post-id-link\" href=\"/posts/");
  }}
	goto _again;
f139:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 208 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    if(sm->thumbnails_left > 0) {
      long post_id = strtol(sm->a1, (char**)&sm->a2, 10);
      g_array_append_val(sm->posts, post_id);
      sm->thumbnails_left -= 1;
      append(sm, true, "<a class=\"dtext-link dtext-id-link dtext-post-id-link thumb-placeholder-link\" data-id=\"");
      append_segment_html_escaped(sm, sm->a1, sm->a2 - 1);
      append(sm, true, "\" href=\"/posts/");
      append_segment_uri_escaped(sm, sm->a1, sm->a2 -1);
      append(sm, true, "\">");
      append(sm, false, "post #");
      append_segment_html_escaped(sm, sm->a1, sm->a2 - 1);
      append(sm, true, "</a>");
    } else {
      append_link(sm, "post #", "<a class=\"dtext-link dtext-id-link dtext-post-id-link\" href=\"/posts/");
    }
  }}
	goto _again;
f135:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 226 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "post changes #", "<a class=\"dtext-link dtext-id-link dtext-post-changes-for-id-link\" href=\"/post_versions?search[post_id]=");
  }}
	goto _again;
f128:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 230 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "flag #", "<a class=\"dtext-link dtext-id-link dtext-post-flag-id-link\" href=\"/post_flags/");
  }}
	goto _again;
f132:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 234 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "note #", "<a class=\"dtext-link dtext-id-link dtext-note-id-link\" href=\"/notes/");
  }}
	goto _again;
f129:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 238 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "forum #", "<a class=\"dtext-link dtext-id-link dtext-forum-post-id-link\" href=\"/forum_posts/");
  }}
	goto _again;
f141:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 242 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "topic #", "<a class=\"dtext-link dtext-id-link dtext-forum-topic-id-link\" href=\"/forum_topics/");
  }}
	goto _again;
f127:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 250 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "comment #", "<a class=\"dtext-link dtext-id-link dtext-comment-id-link\" href=\"/comments/");
  }}
	goto _again;
f133:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 254 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "pool #", "<a class=\"dtext-link dtext-id-link dtext-pool-id-link\" href=\"/pools/");
  }}
	goto _again;
f143:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 258 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "user #", "<a class=\"dtext-link dtext-id-link dtext-user-id-link\" href=\"/users/");
  }}
	goto _again;
f123:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 262 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "artist #", "<a class=\"dtext-link dtext-id-link dtext-artist-id-link\" href=\"/artists/");
  }}
	goto _again;
f124:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 266 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "ban #", "<a class=\"dtext-link dtext-id-link dtext-ban-id-link\" href=\"/bans/");
  }}
	goto _again;
f126:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 270 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "BUR #", "<a class=\"dtext-link dtext-id-link dtext-bulk-update-request-id-link\" href=\"/bulk_update_requests/");
  }}
	goto _again;
f122:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 274 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "alias #", "<a class=\"dtext-link dtext-id-link dtext-tag-alias-id-link\" href=\"/tag_aliases/");
  }}
	goto _again;
f130:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 278 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "implication #", "<a class=\"dtext-link dtext-id-link dtext-tag-implication-id-link\" href=\"/tag_implications/");
  }}
	goto _again;
f131:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 282 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "mod action #", "<a class=\"dtext-link dtext-id-link dtext-mod-action-id-link\" href=\"/mod_actions/");
  }}
	goto _again;
f136:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 286 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "record #", "<a class=\"dtext-link dtext-id-link dtext-user-feedback-id-link\" href=\"/user_feedbacks/");
  }}
	goto _again;
f144:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 290 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "wiki #", "<a class=\"dtext-link dtext-id-link dtext-wiki-page-id-link\" href=\"/wiki_pages/");
  }}
	goto _again;
f137:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 294 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "set #", "<a class=\"dtext-link dtext-id-link dtext-set-id-link\" href=\"/post_sets/");
  }}
	goto _again;
f125:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 298 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "blip #", "<a class=\"dtext-link dtext-id-link dtext-blip-id-link\" href=\"/blips/");
  }}
	goto _again;
f140:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 302 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "ticket #", "<a class=\"dtext-link dtext-id-link dtext-ticket-id-link\" href=\"/tickets/");
  }}
	goto _again;
f138:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 306 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_link(sm, "takedown #", "<a class=\"dtext-link dtext-id-link dtext-takedown-id-link\" href=\"/takedowns/");
  }}
	goto _again;
f120:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 367 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    if (!sm->f_mentions || (sm->a1 > sm->pb && sm->a1 - 1 > sm->pb && sm->a1[-2] != ' ' && sm->a1[-2] != '\r' && sm->a1[-2] != '\n')) {
      // handle emails
      append_c(sm, '@');
      {( sm->p) = (( sm->a1))-1;}
    } else {
      const char* match_end = sm->a2 - 1;
      const char* name_start = sm->a1;
      const char* name_end = find_boundary_c(match_end);

      append(sm, true, "<a rel=\"nofollow\" href=\"/users?name=");
      append_segment_uri_escaped(sm, name_start, name_end);
      append(sm, true, "\">");
      append_c(sm, '@');
      append_segment_html_escaped(sm, name_start, name_end);
      append(sm, true, "</a>");

      if (name_end < match_end) {
        append_segment_html_escaped(sm, name_end + 1, match_end);
      }
    }
  }}
	goto _again;
f119:
#line 83 "ext/dtext/dtext.rl"
	{
  sm->a2 = sm->p;
}
#line 1 "NONE"
	{	switch( ( sm->act) ) {
	case 44:
	{{( sm->p) = ((( sm->te)))-1;}
    const char* match_end = sm->b2;
    const char* url_start = sm->b1;
    const char* url_end = find_boundary_c(match_end);

    if (!append_named_url(sm, url_start, url_end, sm->a1, sm->a2)) {
      {( sm->p)++; goto _out; }
    }

    if (url_end < match_end) {
      append_segment_html_escaped(sm, url_end + 1, match_end);
    }
  }
	break;
	case 46:
	{{( sm->p) = ((( sm->te)))-1;}
    const char* match_end = sm->te - 1;
    const char* url_start = sm->ts;
    const char* url_end = find_boundary_c(match_end);

    append_url(sm, url_start, url_end, url_start, url_end);

    if (url_end < match_end) {
      append_segment_html_escaped(sm, url_end + 1, match_end);
    }
  }
	break;
	case 48:
	{{( sm->p) = ((( sm->te)))-1;}
    append_segment_html_escaped(sm, sm->ts, sm->te - 1);
  }
	break;
	case 49:
	{{( sm->p) = ((( sm->te)))-1;}
    if (!sm->f_mentions || (sm->a1 > sm->pb && sm->a1 - 1 > sm->pb && sm->a1[-2] != ' ' && sm->a1[-2] != '\r' && sm->a1[-2] != '\n')) {
      // handle emails
      append_c(sm, '@');
      {( sm->p) = (( sm->a1))-1;}
    } else {
      const char* match_end = sm->a2 - 1;
      const char* name_start = sm->a1;
      const char* name_end = find_boundary_c(match_end);

      append(sm, true, "<a rel=\"nofollow\" href=\"/users?name=");
      append_segment_uri_escaped(sm, name_start, name_end);
      append(sm, true, "\">");
      append_c(sm, '@');
      append_segment_html_escaped(sm, name_start, name_end);
      append(sm, true, "</a>");

      if (name_end < match_end) {
        append_segment_html_escaped(sm, name_end + 1, match_end);
      }
    }
  }
	break;
	case 85:
	{{( sm->p) = ((( sm->te)))-1;}
    g_debug("inline newline2");
    g_debug("  return");

    if (sm->list_mode) {
      dstack_close_list(sm);
    }

    {( sm->p) = (( sm->ts))-1;}
    { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
  }
	break;
	case 86:
	{{( sm->p) = ((( sm->te)))-1;}
    g_debug("inline newline");

    if (sm->header_mode) {
      sm->header_mode = false;
      dstack_rewind(sm);
      { sm->cs = ( ((int *)sm->stack->data))[--( sm->top)];goto _again;}
    } else if (sm->f_strip) {
      append_c(sm, ' ');
    } else {
      append(sm, true, "<br>");
    }
  }
	break;
	case 88:
	{{( sm->p) = ((( sm->te)))-1;}
    g_debug("inline char: %c", (*( sm->p)));
    append_c_html_escaped(sm, (*( sm->p)));
  }
	break;
	}
	}
	goto _again;
f142:
#line 91 "ext/dtext/dtext.rl"
	{
  sm->b2 = sm->p;
}
#line 246 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_paged_link(sm, "topic #", "<a class=\"dtext-link dtext-id-link dtext-forum-topic-id-link\" href=\"/forum_topics/", "?page=");
  }}
	goto _again;
f114:
#line 91 "ext/dtext/dtext.rl"
	{
  sm->b2 = sm->p;
}
#line 401 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("inline list");

    if (dstack_check(sm, BLOCK_LI)) {
      g_debug("  rewind li");
      dstack_rewind(sm);
    } else if (dstack_check(sm, BLOCK_P)) {
      g_debug("  rewind p");
      dstack_rewind(sm);
    } else if (sm->header_mode) {
      g_debug("  rewind header");
      dstack_rewind(sm);
    }

    g_debug("  next list");
    {( sm->p) = (( sm->ts + 1))-1;}
     sm->cs = 617;
  }}
	goto _again;
f163:
#line 91 "ext/dtext/dtext.rl"
	{
  sm->b2 = sm->p;
}
#line 746 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    int prev_nest = sm->list_nest;
    append_closing_p_if(sm);
    g_debug("list start");
    sm->list_mode = true;
    sm->list_nest = sm->a2 - sm->a1;
    {( sm->p) = (( sm->b1))-1;}

    if (sm->list_nest > prev_nest) {
      int i=0;
      for (i=prev_nest; i<sm->list_nest; ++i) {
        dstack_open_block(sm, BLOCK_UL, "<ul>");
      }
    } else if (sm->list_nest < prev_nest) {
      int i=0;
      for (i=sm->list_nest; i<prev_nest; ++i) {
        if (dstack_check(sm, BLOCK_UL)) {
          g_debug("  dstack pop");
          g_debug("  print </ul>");
          dstack_pop(sm);
          append_block(sm, "</ul>");
        }
      }
    }

    dstack_open_block(sm, BLOCK_LI, "<li>");

    g_debug("  call inline");

    {
  size_t len = sm->stack->len;

  if (len > MAX_STACK_DEPTH) {
    g_set_error_literal(&sm->error, DTEXT_PARSE_ERROR, DTEXT_PARSE_ERROR_DEPTH_EXCEEDED, "too many nested elements");
    {( sm->p)++; goto _out; }
  }

  if (sm->top >= len) {
    g_debug("growing sm->stack %zi\n", len + 16);
    sm->stack = g_array_set_size(sm->stack, len + 16);
  }
{( ((int *)sm->stack->data))[( sm->top)++] =  sm->cs;  sm->cs = 553;goto _again;}}
  }}
	goto _again;
f94:
#line 91 "ext/dtext/dtext.rl"
	{
  sm->b2 = sm->p;
}
#line 999 "ext/dtext/dtext.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("block list");
    g_debug("  call list");
    sm->list_nest = 0;
    sm->list_mode = true;
    append_closing_p_if(sm);
    {( sm->p) = (( sm->ts))-1;}
    {
  size_t len = sm->stack->len;

  if (len > MAX_STACK_DEPTH) {
    g_set_error_literal(&sm->error, DTEXT_PARSE_ERROR, DTEXT_PARSE_ERROR_DEPTH_EXCEEDED, "too many nested elements");
    {( sm->p)++; goto _out; }
  }

  if (sm->top >= len) {
    g_debug("growing sm->stack %zi\n", len + 16);
    sm->stack = g_array_set_size(sm->stack, len + 16);
  }
{( ((int *)sm->stack->data))[( sm->top)++] =  sm->cs;  sm->cs = 617;goto _again;}}
  }}
	goto _again;
f32:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
#line 79 "ext/dtext/dtext.rl"
	{
  sm->a1 = sm->p;
}
	goto _again;
f67:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
#line 346 "ext/dtext/dtext.rl"
	{( sm->act) = 46;}
	goto _again;
f121:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
#line 363 "ext/dtext/dtext.rl"
	{( sm->act) = 48;}
	goto _again;
f31:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
#line 367 "ext/dtext/dtext.rl"
	{( sm->act) = 49;}
	goto _again;
f24:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
#line 612 "ext/dtext/dtext.rl"
	{( sm->act) = 85;}
	goto _again;
f109:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
#line 624 "ext/dtext/dtext.rl"
	{( sm->act) = 86;}
	goto _again;
f110:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
#line 642 "ext/dtext/dtext.rl"
	{( sm->act) = 88;}
	goto _again;
f85:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
#line 779 "ext/dtext/dtext.rl"
	{( sm->act) = 107;}
	goto _again;
f161:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
#line 785 "ext/dtext/dtext.rl"
	{( sm->act) = 108;}
	goto _again;
f1:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
#line 1009 "ext/dtext/dtext.rl"
	{( sm->act) = 126;}
	goto _again;
f90:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
#line 1022 "ext/dtext/dtext.rl"
	{( sm->act) = 127;}
	goto _again;
f118:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
#line 79 "ext/dtext/dtext.rl"
	{
  sm->a1 = sm->p;
}
#line 367 "ext/dtext/dtext.rl"
	{( sm->act) = 49;}
	goto _again;
f27:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
#line 91 "ext/dtext/dtext.rl"
	{
  sm->b2 = sm->p;
}
#line 326 "ext/dtext/dtext.rl"
	{( sm->act) = 44;}
	goto _again;

_again:
	switch ( _dtext_to_state_actions[ sm->cs] ) {
	case 88:
#line 1 "NONE"
	{( sm->ts) = 0;}
	break;
#line 7187 "ext/dtext/dtext.c"
	}

	if ( ++( sm->p) != ( sm->pe) )
		goto _resume;
	_test_eof: {}
	if ( ( sm->p) == ( sm->eof) )
	{
	switch (  sm->cs ) {
	case 533: goto tr0;
	case 0: goto tr0;
	case 534: goto tr628;
	case 535: goto tr628;
	case 1: goto tr2;
	case 536: goto tr629;
	case 537: goto tr629;
	case 2: goto tr2;
	case 538: goto tr628;
	case 3: goto tr2;
	case 4: goto tr2;
	case 5: goto tr2;
	case 539: goto tr632;
	case 540: goto tr634;
	case 541: goto tr628;
	case 6: goto tr2;
	case 7: goto tr2;
	case 8: goto tr2;
	case 9: goto tr2;
	case 10: goto tr2;
	case 11: goto tr2;
	case 12: goto tr2;
	case 13: goto tr2;
	case 14: goto tr2;
	case 15: goto tr2;
	case 16: goto tr2;
	case 17: goto tr2;
	case 18: goto tr2;
	case 542: goto tr642;
	case 19: goto tr2;
	case 20: goto tr2;
	case 21: goto tr2;
	case 22: goto tr2;
	case 23: goto tr2;
	case 24: goto tr2;
	case 25: goto tr2;
	case 543: goto tr643;
	case 26: goto tr2;
	case 27: goto tr2;
	case 28: goto tr2;
	case 29: goto tr2;
	case 30: goto tr2;
	case 544: goto tr644;
	case 31: goto tr2;
	case 32: goto tr2;
	case 33: goto tr2;
	case 34: goto tr2;
	case 35: goto tr2;
	case 36: goto tr2;
	case 37: goto tr2;
	case 38: goto tr2;
	case 39: goto tr2;
	case 40: goto tr2;
	case 41: goto tr2;
	case 42: goto tr2;
	case 43: goto tr2;
	case 44: goto tr2;
	case 45: goto tr2;
	case 46: goto tr2;
	case 47: goto tr2;
	case 48: goto tr2;
	case 545: goto tr645;
	case 546: goto tr647;
	case 49: goto tr2;
	case 50: goto tr2;
	case 547: goto tr648;
	case 548: goto tr650;
	case 51: goto tr2;
	case 52: goto tr2;
	case 53: goto tr2;
	case 54: goto tr2;
	case 55: goto tr2;
	case 56: goto tr2;
	case 57: goto tr2;
	case 549: goto tr651;
	case 58: goto tr2;
	case 59: goto tr2;
	case 60: goto tr2;
	case 61: goto tr2;
	case 62: goto tr2;
	case 63: goto tr2;
	case 550: goto tr628;
	case 552: goto tr655;
	case 64: goto tr77;
	case 65: goto tr77;
	case 66: goto tr77;
	case 67: goto tr77;
	case 68: goto tr77;
	case 69: goto tr77;
	case 70: goto tr77;
	case 71: goto tr77;
	case 72: goto tr77;
	case 73: goto tr77;
	case 74: goto tr77;
	case 75: goto tr77;
	case 76: goto tr77;
	case 77: goto tr77;
	case 78: goto tr77;
	case 554: goto tr685;
	case 555: goto tr688;
	case 79: goto tr100;
	case 80: goto tr102;
	case 81: goto tr102;
	case 556: goto tr689;
	case 557: goto tr689;
	case 558: goto tr691;
	case 559: goto tr692;
	case 82: goto tr107;
	case 83: goto tr107;
	case 84: goto tr107;
	case 85: goto tr107;
	case 86: goto tr100;
	case 560: goto tr694;
	case 87: goto tr100;
	case 88: goto tr100;
	case 89: goto tr107;
	case 90: goto tr107;
	case 91: goto tr107;
	case 92: goto tr107;
	case 93: goto tr107;
	case 94: goto tr107;
	case 95: goto tr107;
	case 96: goto tr107;
	case 97: goto tr107;
	case 98: goto tr107;
	case 99: goto tr107;
	case 100: goto tr107;
	case 101: goto tr107;
	case 102: goto tr107;
	case 103: goto tr107;
	case 104: goto tr107;
	case 105: goto tr107;
	case 106: goto tr107;
	case 107: goto tr107;
	case 108: goto tr107;
	case 561: goto tr692;
	case 109: goto tr107;
	case 110: goto tr107;
	case 111: goto tr107;
	case 112: goto tr107;
	case 113: goto tr107;
	case 114: goto tr107;
	case 115: goto tr107;
	case 116: goto tr107;
	case 117: goto tr107;
	case 118: goto tr107;
	case 119: goto tr107;
	case 120: goto tr107;
	case 121: goto tr107;
	case 122: goto tr107;
	case 123: goto tr107;
	case 124: goto tr107;
	case 125: goto tr107;
	case 562: goto tr692;
	case 126: goto tr100;
	case 563: goto tr701;
	case 127: goto tr100;
	case 128: goto tr100;
	case 564: goto tr703;
	case 565: goto tr692;
	case 129: goto tr107;
	case 130: goto tr107;
	case 131: goto tr107;
	case 132: goto tr107;
	case 133: goto tr107;
	case 134: goto tr107;
	case 566: goto tr707;
	case 135: goto tr107;
	case 136: goto tr107;
	case 137: goto tr107;
	case 138: goto tr107;
	case 139: goto tr107;
	case 140: goto tr107;
	case 141: goto tr107;
	case 567: goto tr709;
	case 568: goto tr692;
	case 142: goto tr107;
	case 143: goto tr107;
	case 144: goto tr107;
	case 145: goto tr107;
	case 569: goto tr714;
	case 146: goto tr107;
	case 147: goto tr107;
	case 148: goto tr107;
	case 149: goto tr107;
	case 150: goto tr107;
	case 570: goto tr716;
	case 151: goto tr107;
	case 152: goto tr107;
	case 153: goto tr107;
	case 154: goto tr107;
	case 571: goto tr718;
	case 572: goto tr692;
	case 155: goto tr107;
	case 156: goto tr107;
	case 157: goto tr107;
	case 158: goto tr107;
	case 159: goto tr107;
	case 160: goto tr107;
	case 161: goto tr107;
	case 162: goto tr107;
	case 573: goto tr721;
	case 574: goto tr692;
	case 163: goto tr107;
	case 164: goto tr107;
	case 165: goto tr107;
	case 166: goto tr107;
	case 167: goto tr107;
	case 575: goto tr725;
	case 168: goto tr107;
	case 169: goto tr107;
	case 170: goto tr107;
	case 171: goto tr107;
	case 172: goto tr107;
	case 173: goto tr107;
	case 576: goto tr727;
	case 577: goto tr692;
	case 174: goto tr107;
	case 175: goto tr107;
	case 176: goto tr107;
	case 177: goto tr107;
	case 178: goto tr107;
	case 179: goto tr107;
	case 180: goto tr107;
	case 181: goto tr107;
	case 182: goto tr107;
	case 183: goto tr107;
	case 184: goto tr107;
	case 185: goto tr107;
	case 578: goto tr730;
	case 579: goto tr692;
	case 186: goto tr107;
	case 187: goto tr107;
	case 188: goto tr107;
	case 189: goto tr107;
	case 190: goto tr107;
	case 191: goto tr107;
	case 192: goto tr107;
	case 193: goto tr107;
	case 194: goto tr107;
	case 195: goto tr107;
	case 196: goto tr107;
	case 580: goto tr733;
	case 581: goto tr692;
	case 197: goto tr107;
	case 198: goto tr107;
	case 199: goto tr107;
	case 200: goto tr107;
	case 201: goto tr107;
	case 582: goto tr736;
	case 583: goto tr692;
	case 202: goto tr107;
	case 203: goto tr107;
	case 204: goto tr107;
	case 205: goto tr107;
	case 206: goto tr107;
	case 584: goto tr739;
	case 207: goto tr107;
	case 208: goto tr107;
	case 209: goto tr107;
	case 210: goto tr107;
	case 585: goto tr741;
	case 211: goto tr107;
	case 212: goto tr107;
	case 213: goto tr107;
	case 214: goto tr107;
	case 215: goto tr107;
	case 216: goto tr107;
	case 217: goto tr107;
	case 218: goto tr107;
	case 219: goto tr107;
	case 586: goto tr743;
	case 587: goto tr692;
	case 220: goto tr107;
	case 221: goto tr107;
	case 222: goto tr107;
	case 223: goto tr107;
	case 224: goto tr107;
	case 225: goto tr107;
	case 226: goto tr107;
	case 588: goto tr746;
	case 589: goto tr692;
	case 227: goto tr107;
	case 228: goto tr107;
	case 229: goto tr107;
	case 230: goto tr107;
	case 590: goto tr749;
	case 591: goto tr692;
	case 231: goto tr107;
	case 232: goto tr107;
	case 233: goto tr107;
	case 234: goto tr107;
	case 235: goto tr107;
	case 236: goto tr107;
	case 237: goto tr107;
	case 238: goto tr107;
	case 239: goto tr107;
	case 240: goto tr107;
	case 592: goto tr755;
	case 241: goto tr107;
	case 242: goto tr107;
	case 243: goto tr107;
	case 244: goto tr107;
	case 245: goto tr107;
	case 246: goto tr107;
	case 247: goto tr107;
	case 248: goto tr107;
	case 249: goto tr107;
	case 250: goto tr107;
	case 251: goto tr107;
	case 252: goto tr107;
	case 253: goto tr107;
	case 254: goto tr107;
	case 593: goto tr757;
	case 255: goto tr107;
	case 256: goto tr107;
	case 257: goto tr107;
	case 258: goto tr107;
	case 259: goto tr107;
	case 260: goto tr107;
	case 261: goto tr107;
	case 594: goto tr759;
	case 262: goto tr107;
	case 263: goto tr107;
	case 264: goto tr107;
	case 265: goto tr107;
	case 266: goto tr107;
	case 267: goto tr107;
	case 595: goto tr761;
	case 268: goto tr304;
	case 269: goto tr304;
	case 596: goto tr764;
	case 597: goto tr692;
	case 270: goto tr107;
	case 271: goto tr107;
	case 272: goto tr107;
	case 273: goto tr107;
	case 274: goto tr107;
	case 598: goto tr767;
	case 599: goto tr692;
	case 275: goto tr107;
	case 276: goto tr107;
	case 277: goto tr107;
	case 278: goto tr107;
	case 279: goto tr107;
	case 600: goto tr770;
	case 601: goto tr692;
	case 280: goto tr107;
	case 281: goto tr107;
	case 282: goto tr107;
	case 283: goto tr107;
	case 284: goto tr107;
	case 285: goto tr107;
	case 286: goto tr107;
	case 287: goto tr107;
	case 602: goto tr783;
	case 288: goto tr107;
	case 289: goto tr107;
	case 290: goto tr107;
	case 291: goto tr107;
	case 292: goto tr107;
	case 293: goto tr107;
	case 294: goto tr107;
	case 295: goto tr107;
	case 296: goto tr107;
	case 603: goto tr784;
	case 297: goto tr107;
	case 298: goto tr107;
	case 299: goto tr107;
	case 300: goto tr107;
	case 301: goto tr107;
	case 302: goto tr107;
	case 303: goto tr107;
	case 304: goto tr107;
	case 305: goto tr107;
	case 306: goto tr107;
	case 307: goto tr107;
	case 308: goto tr107;
	case 309: goto tr107;
	case 310: goto tr107;
	case 311: goto tr107;
	case 312: goto tr107;
	case 313: goto tr107;
	case 314: goto tr107;
	case 315: goto tr107;
	case 316: goto tr107;
	case 317: goto tr107;
	case 318: goto tr107;
	case 604: goto tr785;
	case 319: goto tr107;
	case 320: goto tr107;
	case 321: goto tr107;
	case 322: goto tr107;
	case 323: goto tr107;
	case 324: goto tr107;
	case 325: goto tr107;
	case 326: goto tr107;
	case 327: goto tr107;
	case 328: goto tr107;
	case 329: goto tr107;
	case 330: goto tr107;
	case 331: goto tr107;
	case 332: goto tr107;
	case 333: goto tr107;
	case 334: goto tr107;
	case 335: goto tr107;
	case 336: goto tr107;
	case 337: goto tr107;
	case 338: goto tr107;
	case 339: goto tr107;
	case 340: goto tr107;
	case 341: goto tr107;
	case 342: goto tr107;
	case 343: goto tr107;
	case 344: goto tr107;
	case 345: goto tr107;
	case 346: goto tr107;
	case 347: goto tr107;
	case 348: goto tr107;
	case 349: goto tr107;
	case 350: goto tr107;
	case 351: goto tr107;
	case 352: goto tr107;
	case 353: goto tr107;
	case 354: goto tr107;
	case 355: goto tr107;
	case 356: goto tr107;
	case 357: goto tr107;
	case 358: goto tr107;
	case 359: goto tr107;
	case 360: goto tr107;
	case 361: goto tr107;
	case 362: goto tr107;
	case 363: goto tr107;
	case 364: goto tr107;
	case 365: goto tr107;
	case 366: goto tr107;
	case 367: goto tr107;
	case 368: goto tr107;
	case 369: goto tr107;
	case 370: goto tr107;
	case 371: goto tr107;
	case 372: goto tr107;
	case 373: goto tr107;
	case 374: goto tr107;
	case 375: goto tr107;
	case 376: goto tr107;
	case 377: goto tr107;
	case 378: goto tr107;
	case 379: goto tr107;
	case 380: goto tr107;
	case 381: goto tr107;
	case 382: goto tr107;
	case 383: goto tr107;
	case 384: goto tr107;
	case 385: goto tr107;
	case 386: goto tr107;
	case 387: goto tr107;
	case 388: goto tr107;
	case 389: goto tr107;
	case 390: goto tr107;
	case 391: goto tr107;
	case 392: goto tr107;
	case 393: goto tr107;
	case 394: goto tr107;
	case 395: goto tr107;
	case 396: goto tr107;
	case 397: goto tr107;
	case 398: goto tr107;
	case 399: goto tr107;
	case 400: goto tr107;
	case 401: goto tr107;
	case 402: goto tr107;
	case 403: goto tr107;
	case 404: goto tr107;
	case 405: goto tr107;
	case 406: goto tr107;
	case 407: goto tr107;
	case 408: goto tr107;
	case 409: goto tr107;
	case 410: goto tr107;
	case 411: goto tr107;
	case 412: goto tr107;
	case 413: goto tr107;
	case 414: goto tr107;
	case 415: goto tr107;
	case 416: goto tr107;
	case 417: goto tr107;
	case 418: goto tr107;
	case 419: goto tr107;
	case 420: goto tr107;
	case 421: goto tr107;
	case 422: goto tr107;
	case 423: goto tr107;
	case 424: goto tr107;
	case 425: goto tr107;
	case 426: goto tr107;
	case 427: goto tr107;
	case 428: goto tr107;
	case 429: goto tr107;
	case 430: goto tr107;
	case 431: goto tr107;
	case 432: goto tr107;
	case 433: goto tr107;
	case 434: goto tr107;
	case 435: goto tr107;
	case 436: goto tr107;
	case 437: goto tr107;
	case 438: goto tr107;
	case 439: goto tr107;
	case 440: goto tr107;
	case 441: goto tr107;
	case 442: goto tr107;
	case 443: goto tr107;
	case 444: goto tr107;
	case 445: goto tr107;
	case 446: goto tr107;
	case 447: goto tr107;
	case 448: goto tr107;
	case 449: goto tr107;
	case 450: goto tr107;
	case 451: goto tr107;
	case 452: goto tr107;
	case 453: goto tr107;
	case 454: goto tr107;
	case 455: goto tr107;
	case 456: goto tr107;
	case 457: goto tr107;
	case 458: goto tr107;
	case 459: goto tr107;
	case 460: goto tr107;
	case 461: goto tr107;
	case 462: goto tr107;
	case 463: goto tr107;
	case 464: goto tr107;
	case 465: goto tr107;
	case 466: goto tr107;
	case 467: goto tr107;
	case 468: goto tr107;
	case 469: goto tr107;
	case 470: goto tr107;
	case 471: goto tr107;
	case 472: goto tr107;
	case 605: goto tr692;
	case 606: goto tr692;
	case 473: goto tr107;
	case 474: goto tr107;
	case 475: goto tr107;
	case 476: goto tr107;
	case 477: goto tr107;
	case 478: goto tr107;
	case 479: goto tr100;
	case 607: goto tr788;
	case 480: goto tr100;
	case 481: goto tr100;
	case 482: goto tr107;
	case 608: goto tr692;
	case 483: goto tr107;
	case 484: goto tr107;
	case 485: goto tr107;
	case 486: goto tr107;
	case 487: goto tr107;
	case 488: goto tr107;
	case 489: goto tr107;
	case 610: goto tr793;
	case 612: goto tr797;
	case 490: goto tr563;
	case 491: goto tr563;
	case 492: goto tr563;
	case 493: goto tr563;
	case 494: goto tr563;
	case 614: goto tr801;
	case 495: goto tr569;
	case 496: goto tr569;
	case 497: goto tr569;
	case 498: goto tr569;
	case 499: goto tr569;
	case 500: goto tr569;
	case 501: goto tr569;
	case 502: goto tr569;
	case 616: goto tr805;
	case 503: goto tr578;
	case 504: goto tr578;
	case 505: goto tr578;
	case 506: goto tr578;
	case 507: goto tr578;
	case 508: goto tr578;
	case 509: goto tr578;
	case 510: goto tr578;
	case 511: goto tr578;
	case 512: goto tr578;
	case 513: goto tr578;
	case 514: goto tr578;
	case 515: goto tr578;
	case 516: goto tr578;
	case 517: goto tr578;
	case 518: goto tr578;
	case 519: goto tr578;
	case 520: goto tr578;
	case 521: goto tr578;
	case 522: goto tr578;
	case 523: goto tr578;
	case 524: goto tr578;
	case 525: goto tr578;
	case 526: goto tr578;
	case 527: goto tr578;
	case 528: goto tr578;
	case 618: goto tr612;
	case 529: goto tr612;
	case 619: goto tr813;
	case 620: goto tr813;
	case 530: goto tr614;
	case 621: goto tr814;
	case 622: goto tr814;
	case 531: goto tr614;
	}
	}

	_out: {}
	}

#line 1502 "ext/dtext/dtext.rl"

  dstack_close(sm);

  return sm->error == NULL;
}

/* Everything below is optional, it's only needed to build bin/cdtext.exe. */
#ifdef CDTEXT

static void parse_file(FILE* input, FILE* output, gboolean opt_strip, gboolean opt_inline, gboolean opt_mentions, gboolean opt_color) {
  g_autofree char* dtext = NULL;
  size_t n = 0;

  ssize_t length = getdelim(&dtext, &n, '\0', input);
  if (length == -1) {
    if (ferror(input)) {
      perror("getdelim failed");
      exit(1);
    } else /* EOF (file was empty, continue with the empty string) */ {
      dtext = NULL;
      length = 0;
    }
  }

  StateMachine* sm = init_machine(dtext, length, opt_strip, opt_inline, opt_mentions, opt_color, -1);
  if (!parse_helper(sm)) {
    fprintf(stderr, "dtext parse error: %s\n", sm->error->message);
    exit(1);
  }

  if (fwrite(sm->output->str, 1, sm->output->len, output) != sm->output->len) {
    perror("fwrite failed");
    exit(1);
  }

  free_machine(sm);
}

int main(int argc, char* argv[]) {
  GError* error = NULL;
  gboolean opt_verbose = FALSE;
  gboolean opt_strip = FALSE;
  gboolean opt_inline = FALSE;
  gboolean opt_no_mentions = FALSE;
  gboolean opt_allow_color = FALSE;

  GOptionEntry options[] = {
    { "no-mentions", 'm', 0, G_OPTION_ARG_NONE, &opt_no_mentions, "Don't parse @mentions", NULL },
    { "allow-color", 'c', 0, G_OPTION_ARG_NONE, &opt_allow_color, "Allow color", NULL },
    { "inline",      'i', 0, G_OPTION_ARG_NONE, &opt_inline,      "Parse in inline mode", NULL },
    { "strip",       's', 0, G_OPTION_ARG_NONE, &opt_strip,       "Strip markup", NULL },
    { "verbose",     'v', 0, G_OPTION_ARG_NONE, &opt_verbose,     "Print debug output", NULL },
    { NULL }
  };

  g_autoptr(GOptionContext) context = g_option_context_new("[FILE...]");
  g_option_context_add_main_entries(context, options, NULL);

  if (!g_option_context_parse(context, &argc, &argv, &error)) {
    fprintf(stderr, "option parsing failed: %s\n", error->message);
    g_clear_error(&error);
    return 1;
  }

  if (opt_verbose) {
    g_setenv("G_MESSAGES_DEBUG", "all", TRUE);
  }

  /* skip first argument (progname) */
  argc--, argv++;

  if (argc == 0) {
    parse_file(stdin, stdout, opt_strip, opt_inline, !opt_no_mentions, opt_allow_color);
    return 0;
  }

  for (const char* filename = *argv; argc > 0; argc--, argv++) {
    FILE* input = fopen(filename, "r");
    if (!input) {
      perror("fopen failed");
      return 1;
    }

    parse_file(input, stdout, opt_strip, opt_inline, !opt_no_mentions, opt_allow_color);
    fclose(input);
  }

  return 0;
}

#endif

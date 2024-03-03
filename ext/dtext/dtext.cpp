
#line 1 "ext/dtext/dtext.cpp.rl"
#include "dtext.h"

#include <string.h>
#include <algorithm>
#include <tuple>

#ifdef DEBUG
#undef g_debug
#define STRINGIFY(x) XSTRINGIFY(x)
#define XSTRINGIFY(x) #x
#define g_debug(fmt, ...) fprintf(stderr, "\x1B[1;32mDEBUG\x1B[0m %-28.28s %-24.24s " fmt "\n", __FILE__ ":" STRINGIFY(__LINE__), __func__, ##__VA_ARGS__)
#else
#undef g_debug
#define g_debug(...)
#endif

static const size_t MAX_STACK_DEPTH = 512;

// Characters that mark the end of a link.
//
// http://www.fileformat.info/info/unicode/category/Pe/list.htm
// http://www.fileformat.info/info/unicode/block/cjk_symbols_and_punctuation/list.htm
static char32_t boundary_characters[] = {
  0x0021, // '!' U+0021 EXCLAMATION MARK
  0x0029, // ')' U+0029 RIGHT PARENTHESIS
  0x002C, // ',' U+002C COMMA
  0x002E, // '.' U+002E FULL STOP
  0x003A, // ':' U+003A COLON
  0x003B, // ';' U+003B SEMICOLON
  0x003C, // '<' U+003C LESS-THAN SIGN
  0x003E, // '>' U+003E GREATER-THAN SIGN
  0x003F, // '?' U+003F QUESTION MARK
  0x005D, // ']' U+005D RIGHT SQUARE BRACKET
  0x007D, // '}' U+007D RIGHT CURLY BRACKET
  0x276D, // '❭' U+276D MEDIUM RIGHT-POINTING ANGLE BRACKET ORNAMENT
  0x3000, // '　' U+3000 IDEOGRAPHIC SPACE (U+3000)
  0x3001, // '、' U+3001 IDEOGRAPHIC COMMA (U+3001)
  0x3002, // '。' U+3002 IDEOGRAPHIC FULL STOP (U+3002)
  0x3008, // '〈' U+3008 LEFT ANGLE BRACKET (U+3008)
  0x3009, // '〉' U+3009 RIGHT ANGLE BRACKET (U+3009)
  0x300A, // '《' U+300A LEFT DOUBLE ANGLE BRACKET (U+300A)
  0x300B, // '》' U+300B RIGHT DOUBLE ANGLE BRACKET (U+300B)
  0x300C, // '「' U+300C LEFT CORNER BRACKET (U+300C)
  0x300D, // '」' U+300D RIGHT CORNER BRACKET (U+300D)
  0x300E, // '『' U+300E LEFT WHITE CORNER BRACKET (U+300E)
  0x300F, // '』' U+300F RIGHT WHITE CORNER BRACKET (U+300F)
  0x3010, // '【' U+3010 LEFT BLACK LENTICULAR BRACKET (U+3010)
  0x3011, // '】' U+3011 RIGHT BLACK LENTICULAR BRACKET (U+3011)
  0x3014, // '〔' U+3014 LEFT TORTOISE SHELL BRACKET (U+3014)
  0x3015, // '〕' U+3015 RIGHT TORTOISE SHELL BRACKET (U+3015)
  0x3016, // '〖' U+3016 LEFT WHITE LENTICULAR BRACKET (U+3016)
  0x3017, // '〗' U+3017 RIGHT WHITE LENTICULAR BRACKET (U+3017)
  0x3018, // '〘' U+3018 LEFT WHITE TORTOISE SHELL BRACKET (U+3018)
  0x3019, // '〙' U+3019 RIGHT WHITE TORTOISE SHELL BRACKET (U+3019)
  0x301A, // '〚' U+301A LEFT WHITE SQUARE BRACKET (U+301A)
  0x301B, // '〛' U+301B RIGHT WHITE SQUARE BRACKET (U+301B)
  0x301C, // '〜' U+301C WAVE DASH (U+301C)
  0xFF09, // '）' U+FF09 FULLWIDTH RIGHT PARENTHESIS
  0xFF3D, // '］' U+FF3D FULLWIDTH RIGHT SQUARE BRACKET
  0xFF5D, // '｝' U+FF5D FULLWIDTH RIGHT CURLY BRACKET
  0xFF60, // '｠' U+FF60 FULLWIDTH RIGHT WHITE PARENTHESIS
  0xFF63, // '｣' U+FF63 HALFWIDTH RIGHT CORNER BRACKET
};


#line 672 "ext/dtext/dtext.cpp.rl"



#line 68 "ext/dtext/dtext.cpp"
static const int dtext_start = 507;
static const int dtext_first_final = 507;
static const int dtext_error = -1;

static const int dtext_en_basic_inline = 524;
static const int dtext_en_inline = 526;
static const int dtext_en_inline_code = 580;
static const int dtext_en_code = 582;
static const int dtext_en_table = 584;
static const int dtext_en_main = 507;


#line 675 "ext/dtext/dtext.cpp.rl"

void StateMachine::dstack_push(element_t element) {
  dstack.push_back(element);
}

element_t StateMachine::dstack_pop() {
  if (dstack.empty()) {
    g_debug("dstack pop empty stack");
    return DSTACK_EMPTY;
  } else {
    auto element = dstack.back();
    dstack.pop_back();
    return element;
  }
}

element_t StateMachine::dstack_peek() {
  return dstack.empty() ? DSTACK_EMPTY : dstack.back();
}

bool StateMachine::dstack_check(element_t expected_element) {
  return dstack_peek() == expected_element;
}

// Return true if the given tag is currently open.
bool StateMachine::dstack_is_open(element_t element) {
  return std::find(dstack.begin(), dstack.end(), element) != dstack.end();
}

int StateMachine::dstack_count(element_t element) {
  return std::count(dstack.begin(), dstack.end(), element);
}

void StateMachine::append(const std::string_view c) {
  output += c;
}

void StateMachine::append(const char c) {
  output += c;
}

void StateMachine::append_block(const std::string_view s) {
  if (!options.f_inline) {
    append(s);
  }
}

void StateMachine::append_block(const char s) {
  if (!options.f_inline) {
    append(s);
  }
}

void StateMachine::append_html_escaped(char s) {
  switch (s) {
    case '<': append("&lt;"); break;
    case '>': append("&gt;"); break;
    case '&': append("&amp;"); break;
    case '"': append("&quot;"); break;
    default:  append(s);
  }
}

void StateMachine::append_html_escaped(const std::string_view input) {
  for (const unsigned char c : input) {
    append_html_escaped(c);
  }
}

void StateMachine::append_uri_escaped(const std::string_view uri_part, const char whitelist) {
  static const char hex[] = "0123456789ABCDEF";

  for (const unsigned char c : uri_part) {
    if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' || c == whitelist) {
      append(c);
    } else {
      append('%');
      append(hex[c >> 4]);
      append(hex[c & 0x0F]);
    }
  }
}

void StateMachine::append_url(const char* url) {
  if ((url[0] == '/' || url[0] == '#') && !options.base_url.empty()) {
    append(options.base_url);
  }

  append(url);
}

void StateMachine::append_id_link(const char * title, const char * id_name, const char * url) {
  append("<a class=\"dtext-link dtext-id-link dtext-");
  append(id_name);
  append("-id-link\" href=\"");
  append_url(url);
  append_uri_escaped({ a1, a2 });
  append("\">");
  append(title);
  append(" #");
  append_html_escaped({ a1, a2 });
  append("</a>");
}

void StateMachine::append_unnamed_url(const std::string_view url) {
  append("<a rel=\"nofollow\" class=\"dtext-link\" href=\"");
  append_html_escaped(url);
  append("\">");
  append_html_escaped(url);
  append("</a>");
}

void StateMachine::append_named_url(const std::string_view url, const std::string_view title) {
  auto parsed_title = parse_basic_inline(title);

  if (url[0] == '/' || url[0] == '#') {
    append("<a rel=\"nofollow\" class=\"dtext-link\" href=\"");
    if (!options.base_url.empty()) {
      append(options.base_url);
    }
  } else {
    append("<a rel=\"nofollow\" class=\"dtext-link dtext-external-link\" href=\"");
  }

  append_html_escaped(url);
  append("\">");
  append(parsed_title);
  append("</a>");
}

void StateMachine::append_wiki_link(const std::string_view tag, const std::string_view title) {
  std::string normalized_tag = std::string(tag);
  std::transform(normalized_tag.begin(), normalized_tag.end(), normalized_tag.begin(), [](unsigned char c) { return c == ' ' ? '_' : std::tolower(c); });

  // FIXME: Take the anchor as an argument here
  if (tag[0] == '#') {
    append("<a rel=\"nofollow\" class=\"dtext-link dtext-wiki-link\" href=\"#");
    append_uri_escaped(normalized_tag.substr(1, normalized_tag.size() - 1));
    append("\">");
  } else {
    append("<a rel=\"nofollow\" class=\"dtext-link dtext-wiki-link\" href=\"");
    append_url("/wiki_pages/show_or_new?title=");
    append_uri_escaped(normalized_tag, '#');
    append("\">");
  }
  append_html_escaped(title);
  append("</a>");
}

void StateMachine::append_post_search_link(const std::string_view tag, const std::string_view title) {
  std::string normalized_tag = std::string(tag);
  std::transform(normalized_tag.begin(), normalized_tag.end(), normalized_tag.begin(), [](unsigned char c) { return std::tolower(c); });

  append("<a rel=\"nofollow\" class=\"dtext-link dtext-post-search-link\" href=\"");
  append_url("/posts?tags=");
  append_uri_escaped(normalized_tag);
  append("\">");
  append_html_escaped(title);
  append("</a>");
}

void StateMachine::append_section(const std::string_view summary, bool initially_open) {
  dstack_close_leaf_blocks();
  dstack_open_block(BLOCK_SECTION, "<details");
  if (initially_open) {
     append_block(" open");
  }
  append_block(">");
  append_block("<summary>");
  if (!summary.empty()) {
    append_html_escaped(summary);
  }
  append_block("</summary><div>");
}

void StateMachine::append_closing_p() {
  if (output.size() > 4 && output.ends_with("<br>")) {
    output.resize(output.size() - 4);
  }

  if (output.size() > 3 && output.ends_with("<p>")) {
    output.resize(output.size() - 3);
    return;
  }

  append_block("</p>");
}

void StateMachine::dstack_open_inline(element_t type, const char * html) {
  g_debug("push inline element [%d]: %s", type, html);

  dstack_push(type);
  append(html);
}

void StateMachine::dstack_open_block(element_t type, const char * html) {
  g_debug("push block element [%d]: %s", type, html);

  dstack_push(type);
  append_block(html);
}

void StateMachine::dstack_close_inline(element_t type, const char * close_html) {
  if (dstack_check(type)) {
    g_debug("pop inline element [%d]: %s", type, close_html);

    dstack_pop();
    append(close_html);
  } else {
    g_debug("ignored out-of-order closing inline tag [%d]", type);

    append({ ts, te });
  }
}

bool StateMachine::dstack_close_block(element_t type, const char * close_html) {
  if (dstack_check(type)) {
    g_debug("pop block element [%d]: %s", type, close_html);

    dstack_pop();
    append_block(close_html);
    return true;
  } else {
    g_debug("ignored out-of-order closing block tag [%d]", type);

    append_block({ ts, te });
    return false;
  }
}

// Close the last open tag.
void StateMachine::dstack_rewind() {
  element_t element = dstack_pop();

  switch(element) {
    case BLOCK_P: append_closing_p(); break;
    case INLINE_SPOILER: append("</span>"); break;
    case BLOCK_SPOILER: append_block("</div>"); break;
    case BLOCK_QUOTE: append_block("</blockquote>"); break;
    case BLOCK_SECTION: append_block("</div></details>"); break;
    case BLOCK_CODE: append_block("</pre>"); break;
    case BLOCK_TD: append_block("</td>"); break;
    case BLOCK_TH: append_block("</th>"); break;

    case INLINE_B: append("</strong>"); break;
    case INLINE_I: append("</em>"); break;
    case INLINE_U: append("</u>"); break;
    case INLINE_S: append("</s>"); break;
    case INLINE_SUB: append("</sub>"); break;
    case INLINE_SUP: append("</sup>"); break;
    case INLINE_COLOR: append("</span>"); break;
    case INLINE_VOID: append("</span>"); break;

    case BLOCK_TABLE: append_block("</table>"); break;
    case BLOCK_THEAD: append_block("</thead>"); break;
    case BLOCK_TBODY: append_block("</tbody>"); break;
    case BLOCK_TR: append_block("</tr>"); header_mode = false; break;
    case BLOCK_UL: append_block("</ul>"); header_mode = false; break;
    case BLOCK_LI: append_block("</li>"); header_mode = false; break;
    case BLOCK_H6: append_block("</h6>"); header_mode = false; break;
    case BLOCK_H5: append_block("</h5>"); header_mode = false; break;
    case BLOCK_H4: append_block("</h4>"); header_mode = false; break;
    case BLOCK_H3: append_block("</h3>"); header_mode = false; break;
    case BLOCK_H2: append_block("</h2>"); header_mode = false; break;
    case BLOCK_H1: append_block("</h1>"); header_mode = false; break;

    case DSTACK_EMPTY: break;
  }
}

// Close the last open paragraph or list, if there is one.
void StateMachine::dstack_close_before_block() {
  while (dstack_check(BLOCK_P) || dstack_check(BLOCK_LI) || dstack_check(BLOCK_UL)) {
    dstack_rewind();
  }
}

// Close all remaining open tags.
void StateMachine::dstack_close_all() {
  while (!dstack.empty()) {
    dstack_rewind();
  }
}

// container blocks: [quote], [spoiler], [section]
// leaf blocks: [code], [table], [td]?, [th]?, <h1>, <p>, <li>, <ul>
void StateMachine::dstack_close_leaf_blocks() {
  g_debug("dstack close leaf blocks");

  while (!dstack.empty() && !dstack_check(BLOCK_QUOTE) && !dstack_check(BLOCK_SPOILER) && !dstack_check(BLOCK_SECTION)) {
    dstack_rewind();
  }
}

// Close all open tags up to and including the given tag.
void StateMachine::dstack_close_until(element_t element) {
  while (!dstack.empty() && !dstack_check(element)) {
    dstack_rewind();
  }

  dstack_rewind();
}

void StateMachine::dstack_open_list(int depth) {
  g_debug("open list");

  if (dstack_is_open(BLOCK_LI)) {
    dstack_close_until(BLOCK_LI);
  } else {
    dstack_close_leaf_blocks();
  }

  while (dstack_count(BLOCK_UL) < depth) {
    dstack_open_block(BLOCK_UL, "<ul>");
  }

  while (dstack_count(BLOCK_UL) > depth) {
    dstack_close_until( BLOCK_UL);
  }

  dstack_open_block(BLOCK_LI, "<li>");
}

void StateMachine::dstack_close_list() {
  while (dstack_is_open(BLOCK_UL)) {
    dstack_close_until(BLOCK_UL);
  }
}

static inline std::tuple<char32_t, int> get_utf8_char(const char* c) {
  const unsigned char* p = reinterpret_cast<const unsigned char*>(c);

  // 0x10xxxxxx is a continuation byte; back up to the leading byte.
  while ((p[0] >> 6) == 0b10) {
    p--;
  }

  if (p[0] >> 7 == 0) {
    // 0x0xxxxxxx
    return { p[0], 1 };
  } else if ((p[0] >> 5) == 0b110) {
    // 0x110xxxxx, 0x10xxxxxx
    return { ((p[0] & 0b00011111) << 6) | (p[1] & 0b00111111), 2 };
  } else if ((p[0] >> 4) == 0b1110) {
    // 0x1110xxxx, 0x10xxxxxx, 0x10xxxxxx
    return { ((p[0] & 0b00001111) << 12) | (p[1] & 0b00111111) << 6 | (p[2] & 0b00111111), 3 };
  } else if ((p[0] >> 3) == 0b11110) {
    // 0x11110xxx, 0x10xxxxxx, 0x10xxxxxx, 0x10xxxxxx
    return { ((p[0] & 0b00000111) << 18) | (p[1] & 0b00111111) << 12 | (p[2] & 0b00111111) << 6 | (p[3] & 0b00111111), 4 };
  } else {
    return { 0, 0 };
  }
}

// Returns the preceding non-boundary character if `c` is a boundary character.
// Otherwise, returns `c` if `c` is not a boundary character. Boundary characters
// are trailing punctuation characters that should not be part of the matched text.
static inline const char* find_boundary_c(const char* c) {
  auto [ch, len] = get_utf8_char(c);

  if (std::binary_search(std::begin(boundary_characters), std::end(boundary_characters), ch)) {
    return c - len;
  } else {
    return c;
  }
}

StateMachine::StateMachine(const std::string_view dtext, int initial_state, const DTextOptions options) : options(options) {
  output.reserve(dtext.size() * 1.5);
  stack.reserve(16);
  dstack.reserve(16);
  posts.reserve(10);

  p = dtext.data();
  pb = p;
  pe = p + dtext.size();
  eof = pe;
  cs = initial_state;
}

std::string StateMachine::parse_basic_inline(const std::string_view dtext) {
    DTextOptions options = {};
    options.f_inline = true;
    options.allow_color = false;
    options.max_thumbs = 0;

    StateMachine sm(dtext, dtext_en_basic_inline, options);

    return sm.parse().dtext;
}

DTextResult StateMachine::parse_dtext(const std::string_view dtext, DTextOptions options) {
  StateMachine sm(dtext, dtext_en_main, options);
  return sm.parse();
}

DTextResult StateMachine::parse() {
  StateMachine* sm = this;
  g_debug("start\n");

  
#line 479 "ext/dtext/dtext.cpp"
	{
	( sm->top) = 0;
	( sm->ts) = 0;
	( sm->te) = 0;
	( sm->act) = 0;
	}

#line 1076 "ext/dtext/dtext.cpp.rl"
  
#line 485 "ext/dtext/dtext.cpp"
	{
	short _widec;
	if ( ( sm->p) == ( sm->pe) )
		goto _test_eof;
	goto _resume;

_again:
	switch (  sm->cs ) {
		case 507: goto st507;
		case 508: goto st508;
		case 0: goto st0;
		case 509: goto st509;
		case 510: goto st510;
		case 1: goto st1;
		case 511: goto st511;
		case 512: goto st512;
		case 2: goto st2;
		case 513: goto st513;
		case 3: goto st3;
		case 514: goto st514;
		case 515: goto st515;
		case 4: goto st4;
		case 5: goto st5;
		case 6: goto st6;
		case 7: goto st7;
		case 8: goto st8;
		case 9: goto st9;
		case 10: goto st10;
		case 11: goto st11;
		case 12: goto st12;
		case 13: goto st13;
		case 14: goto st14;
		case 15: goto st15;
		case 16: goto st16;
		case 516: goto st516;
		case 17: goto st17;
		case 18: goto st18;
		case 19: goto st19;
		case 20: goto st20;
		case 21: goto st21;
		case 517: goto st517;
		case 22: goto st22;
		case 23: goto st23;
		case 24: goto st24;
		case 25: goto st25;
		case 26: goto st26;
		case 27: goto st27;
		case 28: goto st28;
		case 29: goto st29;
		case 30: goto st30;
		case 31: goto st31;
		case 32: goto st32;
		case 33: goto st33;
		case 34: goto st34;
		case 35: goto st35;
		case 36: goto st36;
		case 37: goto st37;
		case 38: goto st38;
		case 39: goto st39;
		case 518: goto st518;
		case 519: goto st519;
		case 40: goto st40;
		case 41: goto st41;
		case 520: goto st520;
		case 521: goto st521;
		case 42: goto st42;
		case 43: goto st43;
		case 44: goto st44;
		case 45: goto st45;
		case 46: goto st46;
		case 47: goto st47;
		case 48: goto st48;
		case 522: goto st522;
		case 49: goto st49;
		case 50: goto st50;
		case 51: goto st51;
		case 52: goto st52;
		case 53: goto st53;
		case 523: goto st523;
		case 524: goto st524;
		case 525: goto st525;
		case 54: goto st54;
		case 55: goto st55;
		case 56: goto st56;
		case 57: goto st57;
		case 58: goto st58;
		case 59: goto st59;
		case 60: goto st60;
		case 61: goto st61;
		case 62: goto st62;
		case 63: goto st63;
		case 64: goto st64;
		case 65: goto st65;
		case 66: goto st66;
		case 67: goto st67;
		case 68: goto st68;
		case 69: goto st69;
		case 70: goto st70;
		case 71: goto st71;
		case 72: goto st72;
		case 73: goto st73;
		case 74: goto st74;
		case 75: goto st75;
		case 76: goto st76;
		case 526: goto st526;
		case 527: goto st527;
		case 528: goto st528;
		case 77: goto st77;
		case 78: goto st78;
		case 79: goto st79;
		case 80: goto st80;
		case 81: goto st81;
		case 82: goto st82;
		case 83: goto st83;
		case 84: goto st84;
		case 85: goto st85;
		case 86: goto st86;
		case 87: goto st87;
		case 88: goto st88;
		case 89: goto st89;
		case 90: goto st90;
		case 91: goto st91;
		case 529: goto st529;
		case 530: goto st530;
		case 92: goto st92;
		case 93: goto st93;
		case 531: goto st531;
		case 94: goto st94;
		case 95: goto st95;
		case 96: goto st96;
		case 97: goto st97;
		case 98: goto st98;
		case 99: goto st99;
		case 100: goto st100;
		case 532: goto st532;
		case 101: goto st101;
		case 102: goto st102;
		case 103: goto st103;
		case 104: goto st104;
		case 105: goto st105;
		case 106: goto st106;
		case 107: goto st107;
		case 533: goto st533;
		case 534: goto st534;
		case 535: goto st535;
		case 108: goto st108;
		case 109: goto st109;
		case 110: goto st110;
		case 111: goto st111;
		case 536: goto st536;
		case 112: goto st112;
		case 113: goto st113;
		case 114: goto st114;
		case 115: goto st115;
		case 116: goto st116;
		case 117: goto st117;
		case 118: goto st118;
		case 119: goto st119;
		case 120: goto st120;
		case 121: goto st121;
		case 122: goto st122;
		case 123: goto st123;
		case 124: goto st124;
		case 125: goto st125;
		case 126: goto st126;
		case 127: goto st127;
		case 128: goto st128;
		case 537: goto st537;
		case 129: goto st129;
		case 130: goto st130;
		case 131: goto st131;
		case 132: goto st132;
		case 133: goto st133;
		case 134: goto st134;
		case 135: goto st135;
		case 136: goto st136;
		case 137: goto st137;
		case 538: goto st538;
		case 138: goto st138;
		case 139: goto st139;
		case 140: goto st140;
		case 141: goto st141;
		case 142: goto st142;
		case 143: goto st143;
		case 539: goto st539;
		case 144: goto st144;
		case 145: goto st145;
		case 146: goto st146;
		case 147: goto st147;
		case 148: goto st148;
		case 149: goto st149;
		case 150: goto st150;
		case 540: goto st540;
		case 541: goto st541;
		case 151: goto st151;
		case 152: goto st152;
		case 153: goto st153;
		case 154: goto st154;
		case 542: goto st542;
		case 155: goto st155;
		case 156: goto st156;
		case 157: goto st157;
		case 158: goto st158;
		case 159: goto st159;
		case 543: goto st543;
		case 160: goto st160;
		case 161: goto st161;
		case 162: goto st162;
		case 163: goto st163;
		case 544: goto st544;
		case 545: goto st545;
		case 164: goto st164;
		case 165: goto st165;
		case 166: goto st166;
		case 167: goto st167;
		case 168: goto st168;
		case 169: goto st169;
		case 170: goto st170;
		case 171: goto st171;
		case 546: goto st546;
		case 547: goto st547;
		case 172: goto st172;
		case 173: goto st173;
		case 174: goto st174;
		case 175: goto st175;
		case 176: goto st176;
		case 548: goto st548;
		case 177: goto st177;
		case 178: goto st178;
		case 179: goto st179;
		case 180: goto st180;
		case 181: goto st181;
		case 182: goto st182;
		case 549: goto st549;
		case 550: goto st550;
		case 183: goto st183;
		case 184: goto st184;
		case 185: goto st185;
		case 186: goto st186;
		case 187: goto st187;
		case 188: goto st188;
		case 551: goto st551;
		case 189: goto st189;
		case 552: goto st552;
		case 190: goto st190;
		case 191: goto st191;
		case 192: goto st192;
		case 193: goto st193;
		case 194: goto st194;
		case 195: goto st195;
		case 196: goto st196;
		case 197: goto st197;
		case 198: goto st198;
		case 199: goto st199;
		case 200: goto st200;
		case 201: goto st201;
		case 553: goto st553;
		case 554: goto st554;
		case 202: goto st202;
		case 203: goto st203;
		case 204: goto st204;
		case 205: goto st205;
		case 206: goto st206;
		case 207: goto st207;
		case 208: goto st208;
		case 209: goto st209;
		case 210: goto st210;
		case 211: goto st211;
		case 212: goto st212;
		case 555: goto st555;
		case 556: goto st556;
		case 213: goto st213;
		case 214: goto st214;
		case 215: goto st215;
		case 216: goto st216;
		case 217: goto st217;
		case 557: goto st557;
		case 558: goto st558;
		case 218: goto st218;
		case 219: goto st219;
		case 220: goto st220;
		case 221: goto st221;
		case 222: goto st222;
		case 559: goto st559;
		case 223: goto st223;
		case 224: goto st224;
		case 225: goto st225;
		case 226: goto st226;
		case 560: goto st560;
		case 227: goto st227;
		case 228: goto st228;
		case 229: goto st229;
		case 230: goto st230;
		case 231: goto st231;
		case 232: goto st232;
		case 233: goto st233;
		case 234: goto st234;
		case 235: goto st235;
		case 561: goto st561;
		case 562: goto st562;
		case 236: goto st236;
		case 237: goto st237;
		case 238: goto st238;
		case 239: goto st239;
		case 240: goto st240;
		case 241: goto st241;
		case 242: goto st242;
		case 563: goto st563;
		case 564: goto st564;
		case 243: goto st243;
		case 244: goto st244;
		case 245: goto st245;
		case 246: goto st246;
		case 565: goto st565;
		case 566: goto st566;
		case 247: goto st247;
		case 248: goto st248;
		case 249: goto st249;
		case 250: goto st250;
		case 251: goto st251;
		case 252: goto st252;
		case 253: goto st253;
		case 254: goto st254;
		case 255: goto st255;
		case 256: goto st256;
		case 567: goto st567;
		case 257: goto st257;
		case 258: goto st258;
		case 259: goto st259;
		case 260: goto st260;
		case 261: goto st261;
		case 262: goto st262;
		case 263: goto st263;
		case 264: goto st264;
		case 265: goto st265;
		case 266: goto st266;
		case 267: goto st267;
		case 268: goto st268;
		case 269: goto st269;
		case 270: goto st270;
		case 568: goto st568;
		case 271: goto st271;
		case 272: goto st272;
		case 273: goto st273;
		case 274: goto st274;
		case 275: goto st275;
		case 276: goto st276;
		case 277: goto st277;
		case 569: goto st569;
		case 278: goto st278;
		case 279: goto st279;
		case 280: goto st280;
		case 281: goto st281;
		case 282: goto st282;
		case 283: goto st283;
		case 570: goto st570;
		case 571: goto st571;
		case 284: goto st284;
		case 285: goto st285;
		case 286: goto st286;
		case 287: goto st287;
		case 288: goto st288;
		case 572: goto st572;
		case 573: goto st573;
		case 289: goto st289;
		case 290: goto st290;
		case 291: goto st291;
		case 292: goto st292;
		case 293: goto st293;
		case 574: goto st574;
		case 575: goto st575;
		case 294: goto st294;
		case 295: goto st295;
		case 296: goto st296;
		case 297: goto st297;
		case 298: goto st298;
		case 299: goto st299;
		case 300: goto st300;
		case 301: goto st301;
		case 576: goto st576;
		case 302: goto st302;
		case 303: goto st303;
		case 304: goto st304;
		case 305: goto st305;
		case 306: goto st306;
		case 307: goto st307;
		case 308: goto st308;
		case 309: goto st309;
		case 310: goto st310;
		case 311: goto st311;
		case 312: goto st312;
		case 313: goto st313;
		case 314: goto st314;
		case 577: goto st577;
		case 315: goto st315;
		case 316: goto st316;
		case 317: goto st317;
		case 318: goto st318;
		case 319: goto st319;
		case 320: goto st320;
		case 321: goto st321;
		case 322: goto st322;
		case 323: goto st323;
		case 324: goto st324;
		case 325: goto st325;
		case 326: goto st326;
		case 327: goto st327;
		case 328: goto st328;
		case 329: goto st329;
		case 330: goto st330;
		case 331: goto st331;
		case 332: goto st332;
		case 333: goto st333;
		case 334: goto st334;
		case 335: goto st335;
		case 336: goto st336;
		case 337: goto st337;
		case 338: goto st338;
		case 339: goto st339;
		case 340: goto st340;
		case 341: goto st341;
		case 342: goto st342;
		case 343: goto st343;
		case 344: goto st344;
		case 345: goto st345;
		case 346: goto st346;
		case 347: goto st347;
		case 348: goto st348;
		case 349: goto st349;
		case 350: goto st350;
		case 351: goto st351;
		case 352: goto st352;
		case 353: goto st353;
		case 354: goto st354;
		case 355: goto st355;
		case 356: goto st356;
		case 357: goto st357;
		case 358: goto st358;
		case 359: goto st359;
		case 360: goto st360;
		case 361: goto st361;
		case 362: goto st362;
		case 363: goto st363;
		case 364: goto st364;
		case 365: goto st365;
		case 366: goto st366;
		case 367: goto st367;
		case 368: goto st368;
		case 369: goto st369;
		case 370: goto st370;
		case 371: goto st371;
		case 372: goto st372;
		case 373: goto st373;
		case 374: goto st374;
		case 375: goto st375;
		case 376: goto st376;
		case 377: goto st377;
		case 378: goto st378;
		case 379: goto st379;
		case 380: goto st380;
		case 381: goto st381;
		case 382: goto st382;
		case 383: goto st383;
		case 384: goto st384;
		case 385: goto st385;
		case 386: goto st386;
		case 387: goto st387;
		case 388: goto st388;
		case 389: goto st389;
		case 390: goto st390;
		case 391: goto st391;
		case 392: goto st392;
		case 393: goto st393;
		case 394: goto st394;
		case 395: goto st395;
		case 396: goto st396;
		case 397: goto st397;
		case 398: goto st398;
		case 399: goto st399;
		case 400: goto st400;
		case 401: goto st401;
		case 402: goto st402;
		case 403: goto st403;
		case 404: goto st404;
		case 405: goto st405;
		case 406: goto st406;
		case 407: goto st407;
		case 408: goto st408;
		case 409: goto st409;
		case 410: goto st410;
		case 411: goto st411;
		case 412: goto st412;
		case 413: goto st413;
		case 414: goto st414;
		case 415: goto st415;
		case 416: goto st416;
		case 417: goto st417;
		case 418: goto st418;
		case 419: goto st419;
		case 420: goto st420;
		case 421: goto st421;
		case 422: goto st422;
		case 423: goto st423;
		case 424: goto st424;
		case 425: goto st425;
		case 426: goto st426;
		case 427: goto st427;
		case 428: goto st428;
		case 429: goto st429;
		case 430: goto st430;
		case 431: goto st431;
		case 432: goto st432;
		case 433: goto st433;
		case 434: goto st434;
		case 435: goto st435;
		case 436: goto st436;
		case 437: goto st437;
		case 438: goto st438;
		case 439: goto st439;
		case 440: goto st440;
		case 441: goto st441;
		case 442: goto st442;
		case 443: goto st443;
		case 444: goto st444;
		case 445: goto st445;
		case 446: goto st446;
		case 447: goto st447;
		case 448: goto st448;
		case 449: goto st449;
		case 450: goto st450;
		case 451: goto st451;
		case 452: goto st452;
		case 453: goto st453;
		case 454: goto st454;
		case 455: goto st455;
		case 456: goto st456;
		case 457: goto st457;
		case 458: goto st458;
		case 459: goto st459;
		case 460: goto st460;
		case 461: goto st461;
		case 462: goto st462;
		case 463: goto st463;
		case 464: goto st464;
		case 465: goto st465;
		case 466: goto st466;
		case 467: goto st467;
		case 468: goto st468;
		case 578: goto st578;
		case 579: goto st579;
		case 469: goto st469;
		case 470: goto st470;
		case 471: goto st471;
		case 472: goto st472;
		case 473: goto st473;
		case 474: goto st474;
		case 475: goto st475;
		case 580: goto st580;
		case 581: goto st581;
		case 582: goto st582;
		case 583: goto st583;
		case 476: goto st476;
		case 477: goto st477;
		case 478: goto st478;
		case 479: goto st479;
		case 480: goto st480;
		case 584: goto st584;
		case 585: goto st585;
		case 481: goto st481;
		case 482: goto st482;
		case 483: goto st483;
		case 484: goto st484;
		case 485: goto st485;
		case 486: goto st486;
		case 487: goto st487;
		case 488: goto st488;
		case 489: goto st489;
		case 490: goto st490;
		case 491: goto st491;
		case 492: goto st492;
		case 493: goto st493;
		case 494: goto st494;
		case 495: goto st495;
		case 496: goto st496;
		case 497: goto st497;
		case 498: goto st498;
		case 499: goto st499;
		case 500: goto st500;
		case 501: goto st501;
		case 502: goto st502;
		case 503: goto st503;
		case 504: goto st504;
		case 505: goto st505;
		case 506: goto st506;
	default: break;
	}

	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof;
_resume:
	switch (  sm->cs )
	{
tr0:
#line 1 "NONE"
	{	switch( ( sm->act) ) {
	case 112:
	{{( sm->p) = ((( sm->te)))-1;}
    g_debug("block newline2");

    if (header_mode) {
      dstack_close_leaf_blocks();
    } else if (dstack_is_open(BLOCK_UL)) {
      dstack_close_until(BLOCK_UL);
    } else {
      dstack_close_before_block();
    }
  }
	break;
	case 113:
	{{( sm->p) = ((( sm->te)))-1;}
    g_debug("block newline");
  }
	break;
	}
	}
	goto st507;
tr2:
#line 660 "ext/dtext/dtext.cpp.rl"
	{{( sm->p) = ((( sm->te)))-1;}{
    g_debug("block char: %c", (*( sm->p)));
    ( sm->p)--;

    if (dstack.empty() || dstack_check(BLOCK_QUOTE) || dstack_check(BLOCK_SPOILER) || dstack_check(BLOCK_SECTION)) {
      dstack_open_block(BLOCK_P, "<p>");
    }

    {
  size_t len = stack.size();

  // Should never happen.
  if (len > MAX_STACK_DEPTH) {
    throw DTextError("too many nested elements");
  }

  if (top >= len) {
    g_debug("growing stack %zi\n", len + 16);
    stack.resize(len + 16, 0);
  }
{( (sm->stack.data()))[( sm->top)++] = 507;goto st526;}}
  }}
	goto st507;
tr16:
#line 598 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    g_debug("block [/spoiler]");
    dstack_close_before_block();
    if (dstack_check( BLOCK_SPOILER)) {
      g_debug("  rewind");
      dstack_rewind();
    }
  }}
	goto st507;
tr63:
#line 631 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_close_leaf_blocks();
    dstack_open_block(BLOCK_TABLE, "<table class=\"striped\">");
    {
  size_t len = stack.size();

  // Should never happen.
  if (len > MAX_STACK_DEPTH) {
    throw DTextError("too many nested elements");
  }

  if (top >= len) {
    g_debug("growing stack %zi\n", len + 16);
    stack.resize(len + 16, 0);
  }
{( (sm->stack.data()))[( sm->top)++] = 507;goto st584;}}
  }}
	goto st507;
tr577:
#line 660 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    g_debug("block char: %c", (*( sm->p)));
    ( sm->p)--;

    if (dstack.empty() || dstack_check(BLOCK_QUOTE) || dstack_check(BLOCK_SPOILER) || dstack_check(BLOCK_SECTION)) {
      dstack_open_block(BLOCK_P, "<p>");
    }

    {
  size_t len = stack.size();

  // Should never happen.
  if (len > MAX_STACK_DEPTH) {
    throw DTextError("too many nested elements");
  }

  if (top >= len) {
    g_debug("growing stack %zi\n", len + 16);
    stack.resize(len + 16, 0);
  }
{( (sm->stack.data()))[( sm->top)++] = 507;goto st526;}}
  }}
	goto st507;
tr584:
#line 570 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    append("<span class=\"inline-code\">");
    {
  size_t len = stack.size();

  // Should never happen.
  if (len > MAX_STACK_DEPTH) {
    throw DTextError("too many nested elements");
  }

  if (top >= len) {
    g_debug("growing stack %zi\n", len + 16);
    stack.resize(len + 16, 0);
  }
{( (sm->stack.data()))[( sm->top)++] = 507;goto st580;}}
  }}
	goto st507;
tr586:
#line 660 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("block char: %c", (*( sm->p)));
    ( sm->p)--;

    if (dstack.empty() || dstack_check(BLOCK_QUOTE) || dstack_check(BLOCK_SPOILER) || dstack_check(BLOCK_SECTION)) {
      dstack_open_block(BLOCK_P, "<p>");
    }

    {
  size_t len = stack.size();

  // Should never happen.
  if (len > MAX_STACK_DEPTH) {
    throw DTextError("too many nested elements");
  }

  if (top >= len) {
    g_debug("growing stack %zi\n", len + 16);
    stack.resize(len + 16, 0);
  }
{( (sm->stack.data()))[( sm->top)++] = 507;goto st526;}}
  }}
	goto st507;
tr587:
#line 95 "ext/dtext/dtext.cpp.rl"
	{ b2 = p; }
#line 637 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("block list");
    dstack_open_list(a2 - a1);
    {( sm->p) = (( b1))-1;}
    {
  size_t len = stack.size();

  // Should never happen.
  if (len > MAX_STACK_DEPTH) {
    throw DTextError("too many nested elements");
  }

  if (top >= len) {
    g_debug("growing stack %zi\n", len + 16);
    stack.resize(len + 16, 0);
  }
{( (sm->stack.data()))[( sm->top)++] = 507;goto st526;}}
  }}
	goto st507;
tr590:
#line 575 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    static element_t blocks[] = { BLOCK_H1, BLOCK_H2, BLOCK_H3, BLOCK_H4, BLOCK_H5, BLOCK_H6 };
    char header = *a1;
    element_t block = blocks[header - '1'];

    dstack_open_block(block, "<h");
    append_block(header);
    append_block(">");

    header_mode = true;
    {
  size_t len = stack.size();

  // Should never happen.
  if (len > MAX_STACK_DEPTH) {
    throw DTextError("too many nested elements");
  }

  if (top >= len) {
    g_debug("growing stack %zi\n", len + 16);
    stack.resize(len + 16, 0);
  }
{( (sm->stack.data()))[( sm->top)++] = 507;goto st526;}}
  }}
	goto st507;
tr597:
#line 607 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    dstack_close_leaf_blocks();
    dstack_open_block(BLOCK_CODE, "<pre>");
    {
  size_t len = stack.size();

  // Should never happen.
  if (len > MAX_STACK_DEPTH) {
    throw DTextError("too many nested elements");
  }

  if (top >= len) {
    g_debug("growing stack %zi\n", len + 16);
    stack.resize(len + 16, 0);
  }
{( (sm->stack.data()))[( sm->top)++] = 507;goto st582;}}
  }}
	goto st507;
tr598:
#line 588 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    dstack_close_leaf_blocks();
    dstack_open_block(BLOCK_QUOTE, "<blockquote>");
  }}
	goto st507;
tr599:
#line 626 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("block expanded [section=]");
    append_section({ a1, a2 }, true);
  }}
	goto st507;
tr601:
#line 617 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_section({}, true);
  }}
	goto st507;
tr602:
#line 621 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("block [section=]");
    append_section({ a1, a2 }, false);
  }}
	goto st507;
tr604:
#line 613 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_section({}, false);
  }}
	goto st507;
tr605:
#line 593 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    dstack_close_leaf_blocks();
    dstack_open_block(BLOCK_SPOILER, "<div class=\"spoiler\">");
  }}
	goto st507;
tr606:
#line 566 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    append("`");
  }}
	goto st507;
st507:
#line 1 "NONE"
	{( sm->ts) = 0;}
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof507;
case 507:
#line 1 "NONE"
	{( sm->ts) = ( sm->p);}
#line 1340 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 10: goto tr578;
		case 13: goto st509;
		case 42: goto tr580;
		case 72: goto tr581;
		case 91: goto tr582;
		case 92: goto st523;
		case 96: goto tr584;
		case 104: goto tr581;
	}
	goto tr577;
tr1:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
#line 644 "ext/dtext/dtext.cpp.rl"
	{( sm->act) = 112;}
	goto st508;
tr578:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
#line 656 "ext/dtext/dtext.cpp.rl"
	{( sm->act) = 113;}
	goto st508;
st508:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof508;
case 508:
#line 1363 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 10: goto tr1;
		case 13: goto st0;
	}
	goto tr0;
st0:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof0;
case 0:
	if ( (*( sm->p)) == 10 )
		goto tr1;
	goto tr0;
st509:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof509;
case 509:
	if ( (*( sm->p)) == 10 )
		goto tr578;
	goto tr586;
tr580:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st510;
st510:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof510;
case 510:
#line 1390 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 9: goto tr5;
		case 32: goto tr5;
		case 42: goto st2;
	}
	goto tr586;
tr5:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
	goto st1;
st1:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof1;
case 1:
#line 1403 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 9: goto tr4;
		case 10: goto tr2;
		case 13: goto tr2;
		case 32: goto tr4;
	}
	goto tr3;
tr3:
#line 94 "ext/dtext/dtext.cpp.rl"
	{ b1 = p; }
	goto st511;
st511:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof511;
case 511:
#line 1417 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 10: goto tr587;
		case 13: goto tr587;
	}
	goto st511;
tr4:
#line 94 "ext/dtext/dtext.cpp.rl"
	{ b1 = p; }
	goto st512;
st512:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof512;
case 512:
#line 1429 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 9: goto tr4;
		case 10: goto tr587;
		case 13: goto tr587;
		case 32: goto tr4;
	}
	goto tr3;
st2:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof2;
case 2:
	switch( (*( sm->p)) ) {
		case 9: goto tr5;
		case 32: goto tr5;
		case 42: goto st2;
	}
	goto tr2;
tr581:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st513;
st513:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof513;
case 513:
#line 1453 "ext/dtext/dtext.cpp"
	if ( 49 <= (*( sm->p)) && (*( sm->p)) <= 54 )
		goto tr589;
	goto tr586;
tr589:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st3;
st3:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof3;
case 3:
#line 1463 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 46 )
		goto tr7;
	goto tr2;
tr7:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
	goto st514;
st514:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof514;
case 514:
#line 1473 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 9: goto st514;
		case 32: goto st514;
	}
	goto tr590;
tr582:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st515;
st515:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof515;
case 515:
#line 1485 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 47: goto st4;
		case 67: goto st13;
		case 81: goto st17;
		case 83: goto st22;
		case 84: goto st49;
		case 99: goto st13;
		case 113: goto st17;
		case 115: goto st22;
		case 116: goto st49;
	}
	goto tr586;
st4:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof4;
case 4:
	switch( (*( sm->p)) ) {
		case 83: goto st5;
		case 115: goto st5;
	}
	goto tr2;
st5:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof5;
case 5:
	switch( (*( sm->p)) ) {
		case 80: goto st6;
		case 112: goto st6;
	}
	goto tr2;
st6:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof6;
case 6:
	switch( (*( sm->p)) ) {
		case 79: goto st7;
		case 111: goto st7;
	}
	goto tr2;
st7:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof7;
case 7:
	switch( (*( sm->p)) ) {
		case 73: goto st8;
		case 105: goto st8;
	}
	goto tr2;
st8:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof8;
case 8:
	switch( (*( sm->p)) ) {
		case 76: goto st9;
		case 108: goto st9;
	}
	goto tr2;
st9:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof9;
case 9:
	switch( (*( sm->p)) ) {
		case 69: goto st10;
		case 101: goto st10;
	}
	goto tr2;
st10:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof10;
case 10:
	switch( (*( sm->p)) ) {
		case 82: goto st11;
		case 114: goto st11;
	}
	goto tr2;
st11:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof11;
case 11:
	switch( (*( sm->p)) ) {
		case 83: goto st12;
		case 93: goto tr16;
		case 115: goto st12;
	}
	goto tr2;
st12:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof12;
case 12:
	if ( (*( sm->p)) == 93 )
		goto tr16;
	goto tr2;
st13:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof13;
case 13:
	switch( (*( sm->p)) ) {
		case 79: goto st14;
		case 111: goto st14;
	}
	goto tr2;
st14:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof14;
case 14:
	switch( (*( sm->p)) ) {
		case 68: goto st15;
		case 100: goto st15;
	}
	goto tr2;
st15:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof15;
case 15:
	switch( (*( sm->p)) ) {
		case 69: goto st16;
		case 101: goto st16;
	}
	goto tr2;
st16:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof16;
case 16:
	if ( (*( sm->p)) == 93 )
		goto st516;
	goto tr2;
st516:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof516;
case 516:
	if ( (*( sm->p)) == 32 )
		goto st516;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto st516;
	goto tr597;
st17:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof17;
case 17:
	switch( (*( sm->p)) ) {
		case 85: goto st18;
		case 117: goto st18;
	}
	goto tr2;
st18:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof18;
case 18:
	switch( (*( sm->p)) ) {
		case 79: goto st19;
		case 111: goto st19;
	}
	goto tr2;
st19:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof19;
case 19:
	switch( (*( sm->p)) ) {
		case 84: goto st20;
		case 116: goto st20;
	}
	goto tr2;
st20:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof20;
case 20:
	switch( (*( sm->p)) ) {
		case 69: goto st21;
		case 101: goto st21;
	}
	goto tr2;
st21:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof21;
case 21:
	if ( (*( sm->p)) == 93 )
		goto st517;
	goto tr2;
st517:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof517;
case 517:
	if ( (*( sm->p)) == 32 )
		goto st517;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto st517;
	goto tr598;
st22:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof22;
case 22:
	switch( (*( sm->p)) ) {
		case 69: goto st23;
		case 80: goto st42;
		case 101: goto st23;
		case 112: goto st42;
	}
	goto tr2;
st23:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof23;
case 23:
	switch( (*( sm->p)) ) {
		case 67: goto st24;
		case 99: goto st24;
	}
	goto tr2;
st24:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof24;
case 24:
	switch( (*( sm->p)) ) {
		case 84: goto st25;
		case 116: goto st25;
	}
	goto tr2;
st25:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof25;
case 25:
	switch( (*( sm->p)) ) {
		case 73: goto st26;
		case 105: goto st26;
	}
	goto tr2;
st26:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof26;
case 26:
	switch( (*( sm->p)) ) {
		case 79: goto st27;
		case 111: goto st27;
	}
	goto tr2;
st27:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof27;
case 27:
	switch( (*( sm->p)) ) {
		case 78: goto st28;
		case 110: goto st28;
	}
	goto tr2;
st28:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof28;
case 28:
	switch( (*( sm->p)) ) {
		case 44: goto st29;
		case 61: goto st40;
		case 93: goto st521;
	}
	goto tr2;
st29:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof29;
case 29:
	switch( (*( sm->p)) ) {
		case 69: goto st30;
		case 101: goto st30;
	}
	goto tr2;
st30:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof30;
case 30:
	switch( (*( sm->p)) ) {
		case 88: goto st31;
		case 120: goto st31;
	}
	goto tr2;
st31:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof31;
case 31:
	switch( (*( sm->p)) ) {
		case 80: goto st32;
		case 112: goto st32;
	}
	goto tr2;
st32:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof32;
case 32:
	switch( (*( sm->p)) ) {
		case 65: goto st33;
		case 97: goto st33;
	}
	goto tr2;
st33:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof33;
case 33:
	switch( (*( sm->p)) ) {
		case 78: goto st34;
		case 110: goto st34;
	}
	goto tr2;
st34:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof34;
case 34:
	switch( (*( sm->p)) ) {
		case 68: goto st35;
		case 100: goto st35;
	}
	goto tr2;
st35:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof35;
case 35:
	switch( (*( sm->p)) ) {
		case 69: goto st36;
		case 101: goto st36;
	}
	goto tr2;
st36:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof36;
case 36:
	switch( (*( sm->p)) ) {
		case 68: goto st37;
		case 100: goto st37;
	}
	goto tr2;
st37:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof37;
case 37:
	switch( (*( sm->p)) ) {
		case 61: goto st38;
		case 93: goto st519;
	}
	goto tr2;
st38:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof38;
case 38:
	if ( (*( sm->p)) == 93 )
		goto tr2;
	goto tr46;
tr46:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st39;
st39:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof39;
case 39:
#line 1833 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 93 )
		goto tr48;
	goto st39;
tr48:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
	goto st518;
st518:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof518;
case 518:
#line 1843 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 32 )
		goto st518;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto st518;
	goto tr599;
st519:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof519;
case 519:
	if ( (*( sm->p)) == 32 )
		goto st519;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto st519;
	goto tr601;
st40:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof40;
case 40:
	if ( (*( sm->p)) == 93 )
		goto tr2;
	goto tr49;
tr49:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st41;
st41:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof41;
case 41:
#line 1871 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 93 )
		goto tr51;
	goto st41;
tr51:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
	goto st520;
st520:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof520;
case 520:
#line 1881 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 32 )
		goto st520;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto st520;
	goto tr602;
st521:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof521;
case 521:
	if ( (*( sm->p)) == 32 )
		goto st521;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto st521;
	goto tr604;
st42:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof42;
case 42:
	switch( (*( sm->p)) ) {
		case 79: goto st43;
		case 111: goto st43;
	}
	goto tr2;
st43:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof43;
case 43:
	switch( (*( sm->p)) ) {
		case 73: goto st44;
		case 105: goto st44;
	}
	goto tr2;
st44:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof44;
case 44:
	switch( (*( sm->p)) ) {
		case 76: goto st45;
		case 108: goto st45;
	}
	goto tr2;
st45:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof45;
case 45:
	switch( (*( sm->p)) ) {
		case 69: goto st46;
		case 101: goto st46;
	}
	goto tr2;
st46:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof46;
case 46:
	switch( (*( sm->p)) ) {
		case 82: goto st47;
		case 114: goto st47;
	}
	goto tr2;
st47:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof47;
case 47:
	switch( (*( sm->p)) ) {
		case 83: goto st48;
		case 93: goto st522;
		case 115: goto st48;
	}
	goto tr2;
st48:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof48;
case 48:
	if ( (*( sm->p)) == 93 )
		goto st522;
	goto tr2;
st522:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof522;
case 522:
	if ( (*( sm->p)) == 32 )
		goto st522;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto st522;
	goto tr605;
st49:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof49;
case 49:
	switch( (*( sm->p)) ) {
		case 65: goto st50;
		case 97: goto st50;
	}
	goto tr2;
st50:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof50;
case 50:
	switch( (*( sm->p)) ) {
		case 66: goto st51;
		case 98: goto st51;
	}
	goto tr2;
st51:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof51;
case 51:
	switch( (*( sm->p)) ) {
		case 76: goto st52;
		case 108: goto st52;
	}
	goto tr2;
st52:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof52;
case 52:
	switch( (*( sm->p)) ) {
		case 69: goto st53;
		case 101: goto st53;
	}
	goto tr2;
st53:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof53;
case 53:
	if ( (*( sm->p)) == 93 )
		goto tr63;
	goto tr2;
st523:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof523;
case 523:
	if ( (*( sm->p)) == 96 )
		goto tr606;
	goto tr586;
tr64:
#line 187 "ext/dtext/dtext.cpp.rl"
	{{( sm->p) = ((( sm->te)))-1;}{ append_html_escaped((*( sm->p))); }}
	goto st524;
tr70:
#line 174 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(INLINE_B, "</strong>"); }}
	goto st524;
tr71:
#line 176 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(INLINE_I, "</em>"); }}
	goto st524;
tr73:
#line 178 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(INLINE_S, "</s>"); }}
	goto st524;
tr76:
#line 184 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(INLINE_SUB, "</sub>"); }}
	goto st524;
tr77:
#line 182 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(INLINE_SUP, "</sup>"); }}
	goto st524;
tr78:
#line 180 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(INLINE_U, "</u>"); }}
	goto st524;
tr82:
#line 186 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(INLINE_VOID, "</span>"); }}
	goto st524;
tr83:
#line 173 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(INLINE_B, "<strong>"); }}
	goto st524;
tr84:
#line 175 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(INLINE_I, "<em>"); }}
	goto st524;
tr86:
#line 177 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(INLINE_S, "<s>"); }}
	goto st524;
tr89:
#line 183 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(INLINE_SUB, "<sub>"); }}
	goto st524;
tr90:
#line 181 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(INLINE_SUP, "<sup>"); }}
	goto st524;
tr91:
#line 179 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(INLINE_U, "<u>"); }}
	goto st524;
tr95:
#line 185 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(INLINE_VOID, "<span class=\"user-owner with-stuyle\">"); }}
	goto st524;
tr607:
#line 187 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ append_html_escaped((*( sm->p))); }}
	goto st524;
tr609:
#line 187 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_html_escaped((*( sm->p))); }}
	goto st524;
st524:
#line 1 "NONE"
	{( sm->ts) = 0;}
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof524;
case 524:
#line 1 "NONE"
	{( sm->ts) = ( sm->p);}
#line 2073 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 91 )
		goto tr608;
	goto tr607;
tr608:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st525;
st525:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof525;
case 525:
#line 2083 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 47: goto st54;
		case 66: goto st66;
		case 73: goto st67;
		case 83: goto st68;
		case 85: goto st72;
		case 86: goto st73;
		case 98: goto st66;
		case 105: goto st67;
		case 115: goto st68;
		case 117: goto st72;
		case 118: goto st73;
	}
	goto tr609;
st54:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof54;
case 54:
	switch( (*( sm->p)) ) {
		case 66: goto st55;
		case 73: goto st56;
		case 83: goto st57;
		case 85: goto st61;
		case 86: goto st62;
		case 98: goto st55;
		case 105: goto st56;
		case 115: goto st57;
		case 117: goto st61;
		case 118: goto st62;
	}
	goto tr64;
st55:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof55;
case 55:
	if ( (*( sm->p)) == 93 )
		goto tr70;
	goto tr64;
st56:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof56;
case 56:
	if ( (*( sm->p)) == 93 )
		goto tr71;
	goto tr64;
st57:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof57;
case 57:
	switch( (*( sm->p)) ) {
		case 85: goto st58;
		case 93: goto tr73;
		case 117: goto st58;
	}
	goto tr64;
st58:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof58;
case 58:
	switch( (*( sm->p)) ) {
		case 66: goto st59;
		case 80: goto st60;
		case 98: goto st59;
		case 112: goto st60;
	}
	goto tr64;
st59:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof59;
case 59:
	if ( (*( sm->p)) == 93 )
		goto tr76;
	goto tr64;
st60:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof60;
case 60:
	if ( (*( sm->p)) == 93 )
		goto tr77;
	goto tr64;
st61:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof61;
case 61:
	if ( (*( sm->p)) == 93 )
		goto tr78;
	goto tr64;
st62:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof62;
case 62:
	switch( (*( sm->p)) ) {
		case 79: goto st63;
		case 111: goto st63;
	}
	goto tr64;
st63:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof63;
case 63:
	switch( (*( sm->p)) ) {
		case 73: goto st64;
		case 105: goto st64;
	}
	goto tr64;
st64:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof64;
case 64:
	switch( (*( sm->p)) ) {
		case 68: goto st65;
		case 100: goto st65;
	}
	goto tr64;
st65:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof65;
case 65:
	if ( (*( sm->p)) == 93 )
		goto tr82;
	goto tr64;
st66:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof66;
case 66:
	if ( (*( sm->p)) == 93 )
		goto tr83;
	goto tr64;
st67:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof67;
case 67:
	if ( (*( sm->p)) == 93 )
		goto tr84;
	goto tr64;
st68:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof68;
case 68:
	switch( (*( sm->p)) ) {
		case 85: goto st69;
		case 93: goto tr86;
		case 117: goto st69;
	}
	goto tr64;
st69:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof69;
case 69:
	switch( (*( sm->p)) ) {
		case 66: goto st70;
		case 80: goto st71;
		case 98: goto st70;
		case 112: goto st71;
	}
	goto tr64;
st70:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof70;
case 70:
	if ( (*( sm->p)) == 93 )
		goto tr89;
	goto tr64;
st71:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof71;
case 71:
	if ( (*( sm->p)) == 93 )
		goto tr90;
	goto tr64;
st72:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof72;
case 72:
	if ( (*( sm->p)) == 93 )
		goto tr91;
	goto tr64;
st73:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof73;
case 73:
	switch( (*( sm->p)) ) {
		case 79: goto st74;
		case 111: goto st74;
	}
	goto tr64;
st74:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof74;
case 74:
	switch( (*( sm->p)) ) {
		case 73: goto st75;
		case 105: goto st75;
	}
	goto tr64;
st75:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof75;
case 75:
	switch( (*( sm->p)) ) {
		case 68: goto st76;
		case 100: goto st76;
	}
	goto tr64;
st76:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof76;
case 76:
	if ( (*( sm->p)) == 93 )
		goto tr95;
	goto tr64;
tr96:
#line 1 "NONE"
	{	switch( ( sm->act) ) {
	case 80:
	{{( sm->p) = ((( sm->te)))-1;}
    g_debug("inline newline2");
    g_debug("  return");

    dstack_close_list();

    {( sm->p) = (( ts))-1;}
    { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
  }
	break;
	case 81:
	{{( sm->p) = ((( sm->te)))-1;}
    g_debug("inline newline");

    if (header_mode) {
      dstack_close_leaf_blocks();
      { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
    } else if (dstack_is_open(BLOCK_UL)) {
      dstack_close_list();
      { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
    } else {
      append("<br>");
    }
  }
	break;
	case 83:
	{{( sm->p) = ((( sm->te)))-1;}
    g_debug("inline char: %c", (*( sm->p)));
    append_html_escaped((*( sm->p)));
  }
	break;
	}
	}
	goto st526;
tr98:
#line 457 "ext/dtext/dtext.cpp.rl"
	{{( sm->p) = ((( sm->te)))-1;}{
    g_debug("inline newline2");
    g_debug("  return");

    dstack_close_list();

    {( sm->p) = (( ts))-1;}
    { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
  }}
	goto st526;
tr109:
#line 355 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    g_debug("inline [/spoiler]");
    dstack_close_before_block();

    if (dstack_check(INLINE_SPOILER)) {
      dstack_close_inline(INLINE_SPOILER, "</span>");
    } else if (dstack_close_block(BLOCK_SPOILER, "</div>")) {
      { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
    }
  }}
	goto st526;
tr111:
#line 451 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    if (dstack_close_block(BLOCK_TD, "</td>")) {
      { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
    }
  }}
	goto st526;
tr112:
#line 467 "ext/dtext/dtext.cpp.rl"
	{{( sm->p) = ((( sm->te)))-1;}{
    g_debug("inline newline");

    if (header_mode) {
      dstack_close_leaf_blocks();
      { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
    } else if (dstack_is_open(BLOCK_UL)) {
      dstack_close_list();
      { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
    } else {
      append("<br>");
    }
  }}
	goto st526;
tr134:
#line 485 "ext/dtext/dtext.cpp.rl"
	{{( sm->p) = ((( sm->te)))-1;}{
    g_debug("inline char: %c", (*( sm->p)));
    append_html_escaped((*( sm->p)));
  }}
	goto st526;
tr152:
#line 95 "ext/dtext/dtext.cpp.rl"
	{ b2 = p; }
#line 276 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    append_named_url({ b1, b2 }, { a1, a2 });
  }}
	goto st526;
tr168:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 292 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    append_unnamed_url({ a1, a2 });
  }}
	goto st526;
tr329:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 200 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    append("<a id=\"");
    std::string lowercased_tag = std::string(a1, a2 - a1);
    std::transform(lowercased_tag.begin(), lowercased_tag.end(), lowercased_tag.begin(), [](unsigned char c) { return std::tolower(c); });
    append_uri_escaped(lowercased_tag);
    append("\"></a>");
  }}
	goto st526;
tr337:
#line 303 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(INLINE_B, "</strong>"); }}
	goto st526;
tr345:
#line 344 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    if(options.allow_color) {
      dstack_close_inline(INLINE_COLOR, "</span>");
    }
    {goto st526;}
  }}
	goto st526;
tr346:
#line 305 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(INLINE_I, "</em>"); }}
	goto st526;
tr348:
#line 307 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(INLINE_S, "</s>"); }}
	goto st526;
tr351:
#line 313 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(INLINE_SUB, "</sub>"); }}
	goto st526;
tr352:
#line 311 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(INLINE_SUP, "</sup>"); }}
	goto st526;
tr359:
#line 445 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    if (dstack_close_block(BLOCK_TH, "</th>")) {
      { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
    }
  }}
	goto st526;
tr360:
#line 309 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(INLINE_U, "</u>"); }}
	goto st526;
tr364:
#line 315 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_close_inline(INLINE_VOID, "</span>"); }}
	goto st526;
tr365:
#line 302 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(INLINE_B, "<strong>"); }}
	goto st526;
tr370:
#line 397 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_close_before_block();
    {( sm->p) = (( ts))-1;}
    { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
  }}
	goto st526;
tr392:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 329 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    if(options.allow_color) {
      dstack_push(INLINE_COLOR);
      append("<span class=\"dtext-color\" style=\"color:");
      if(a1[0] == '#') {
        append("#");
        append_uri_escaped({ a1 + 1, a2 });
      } else {
        append_uri_escaped({ a1, a2 });
      }
      append("\">");
    }
    {goto st526;}
  }}
	goto st526;
tr398:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 317 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    if(options.allow_color) {
      dstack_push(INLINE_COLOR);
      append("<span class=\"dtext-color-");
      append_uri_escaped({ a1, a2 });
      append("\">");
    }
    {goto st526;}
  }}
	goto st526;
tr465:
#line 304 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(INLINE_I, "<em>"); }}
	goto st526;
tr470:
#line 419 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    g_debug("inline [quote]");
    dstack_close_leaf_blocks();
    {( sm->p) = (( ts))-1;}
    { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
  }}
	goto st526;
tr474:
#line 306 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(INLINE_S, "<s>"); }}
	goto st526;
tr482:
#line 432 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    g_debug("inline [section]");
    dstack_close_leaf_blocks();
    {( sm->p) = (( ts))-1;}
    { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
  }}
	goto st526;
tr493:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 432 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    g_debug("inline [section]");
    dstack_close_leaf_blocks();
    {( sm->p) = (( ts))-1;}
    { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
  }}
	goto st526;
tr500:
#line 351 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_open_inline(INLINE_SPOILER, "<span class=\"spoiler\">");
  }}
	goto st526;
tr503:
#line 312 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(INLINE_SUB, "<sub>"); }}
	goto st526;
tr504:
#line 310 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(INLINE_SUP, "<sup>"); }}
	goto st526;
tr509:
#line 375 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_close_before_block();
    {( sm->p) = (( ts))-1;}
    { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
  }}
	goto st526;
tr510:
#line 308 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(INLINE_U, "<u>"); }}
	goto st526;
tr514:
#line 314 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{ dstack_open_inline(INLINE_VOID, "<span class=\"user-owner with-stuyle\">"); }}
	goto st526;
tr520:
#line 256 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    append_wiki_link({ a1, a2 }, { a1, a2 });
  }}
	goto st526;
tr524:
#line 260 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    append_wiki_link({ a1, a2 }, { b1, b2 });
  }}
	goto st526;
tr534:
#line 252 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    append_post_search_link({ a1, a2 }, { b1, b2 });
  }}
	goto st526;
tr535:
#line 248 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    append_post_search_link({ a1, a2 }, { a1, a2 });
  }}
	goto st526;
tr616:
#line 485 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    g_debug("inline char: %c", (*( sm->p)));
    append_html_escaped((*( sm->p)));
  }}
	goto st526;
tr637:
#line 195 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    append("<span class=\"inline-code\">");
    {
  size_t len = stack.size();

  // Should never happen.
  if (len > MAX_STACK_DEPTH) {
    throw DTextError("too many nested elements");
  }

  if (top >= len) {
    g_debug("growing stack %zi\n", len + 16);
    stack.resize(len + 16, 0);
  }
{( (sm->stack.data()))[( sm->top)++] = 526;goto st580;}}
  }}
	goto st526;
tr639:
#line 467 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("inline newline");

    if (header_mode) {
      dstack_close_leaf_blocks();
      { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
    } else if (dstack_is_open(BLOCK_UL)) {
      dstack_close_list();
      { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
    } else {
      append("<br>");
    }
  }}
	goto st526;
tr644:
#line 457 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("inline newline2");
    g_debug("  return");

    dstack_close_list();

    {( sm->p) = (( ts))-1;}
    { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
  }}
	goto st526;
tr646:
#line 95 "ext/dtext/dtext.cpp.rl"
	{ b2 = p; }
#line 296 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("inline list");
    {( sm->p) = (( ts + 1))-1;}
    { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
  }}
	goto st526;
tr648:
#line 369 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    dstack_close_leaf_blocks();
    {( sm->p) = (( ts))-1;}
    { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
  }}
	goto st526;
tr650:
#line 426 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("inline [/quote]");
    dstack_close_until(BLOCK_QUOTE);
    { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
  }}
	goto st526;
tr651:
#line 439 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("inline [/expand]");
    dstack_close_until(BLOCK_SECTION);
    { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
  }}
	goto st526;
tr652:
#line 481 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append(' ');
  }}
	goto st526;
tr653:
#line 485 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("inline char: %c", (*( sm->p)));
    append_html_escaped((*( sm->p)));
  }}
	goto st526;
tr655:
#line 95 "ext/dtext/dtext.cpp.rl"
	{ b2 = p; }
#line 264 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    const char* match_end = b2;
    const char* url_start = b1;
    const char* url_end = find_boundary_c(match_end - 1) + 1;

    append_named_url({ url_start, url_end }, { a1, a2 });

    if (url_end < match_end) {
      append_html_escaped({ url_end, match_end });
    }
  }}
	goto st526;
tr659:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 238 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("alias", "tag-alias", "/tag_aliases/"); }}
	goto st526;
tr661:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 235 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("artist", "artist", "/artists/"); }}
	goto st526;
tr666:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 236 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("ban", "ban", "/bans/"); }}
	goto st526;
tr668:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 244 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("blip", "blip", "/blips/"); }}
	goto st526;
tr670:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 237 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("BUR", "bulk-update-request", "/bulk_update_requests/"); }}
	goto st526;
tr673:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 232 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("comment", "comment", "/comments/"); }}
	goto st526;
tr677:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 228 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("flag", "post-flag", "/post_flags/"); }}
	goto st526;
tr679:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 230 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("forum", "forum-post", "/forum_posts/"); }}
	goto st526;
tr682:
#line 280 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    const char* match_end = te;
    const char* url_start = ts;
    const char* url_end = find_boundary_c(match_end - 1) + 1;

    append_unnamed_url({ url_start, url_end });

    if (url_end < match_end) {
      append_html_escaped({ url_end, match_end });
    }
  }}
	goto st526;
tr684:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 239 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("implication", "tag-implication", "/tag_implications/"); }}
	goto st526;
tr687:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 240 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("mod action", "mod-action", "/mod_actions/"); }}
	goto st526;
tr690:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 229 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("note", "note", "/notes/"); }}
	goto st526;
tr693:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 233 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("pool", "pool", "/pools/"); }}
	goto st526;
tr695:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 226 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("post", "post", "/posts/"); }}
	goto st526;
tr697:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 227 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("post changes", "post-changes-for", "/post_versions?search[post_id]="); }}
	goto st526;
tr700:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 241 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("record", "user-feedback", "/user_feedbacks/"); }}
	goto st526;
tr703:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 243 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("set", "set", "/post_sets/"); }}
	goto st526;
tr709:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 246 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("takedown", "takedown", "/takedowns/"); }}
	goto st526;
tr711:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 208 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    if(posts.size() < options.max_thumbs) {
      long post_id = strtol(a1, (char**)&a2, 10);
      posts.push_back(post_id);
      append("<a class=\"dtext-link dtext-id-link dtext-post-id-link thumb-placeholder-link\" data-id=\"");
      append_html_escaped({ a1, a2 });
      append("\" href=\"");
      append_url("/posts/");
      append_uri_escaped({ a1, a2 });
      append("\">");
      append("post #");
      append_html_escaped({ a1, a2 });
      append("</a>");
    } else {
      append_id_link("post", "post", "/posts/");
    }
  }}
	goto st526;
tr713:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 245 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("ticket", "ticket", "/tickets/"); }}
	goto st526;
tr715:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 231 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("topic", "forum-topic", "/forum_topics/"); }}
	goto st526;
tr718:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 234 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("user", "user", "/users/"); }}
	goto st526;
tr721:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
#line 242 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{ append_id_link("wiki", "wiki-page", "/wiki_pages/"); }}
	goto st526;
tr734:
#line 403 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("inline [/code]");
    dstack_close_before_block();

    if (dstack_check(BLOCK_LI)) {
      dstack_close_list();
    }

    if (dstack_check(BLOCK_CODE)) {
      dstack_rewind();
      { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
    } else {
      append_block("[/code]");
    }
  }}
	goto st526;
tr735:
#line 381 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    g_debug("inline [/table]");
    dstack_close_before_block();

    if (dstack_check(BLOCK_LI)) {
      dstack_close_list();
    }

    if (dstack_check(BLOCK_TABLE)) {
      dstack_rewind();
      { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
    } else {
      append_block("[/table]");
    }
  }}
	goto st526;
tr736:
#line 191 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    append("`");
  }}
	goto st526;
st526:
#line 1 "NONE"
	{( sm->ts) = 0;}
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof526;
case 526:
#line 1 "NONE"
	{( sm->ts) = ( sm->p);}
#line 2820 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 10: goto tr617;
		case 13: goto st534;
		case 34: goto tr619;
		case 60: goto tr620;
		case 65: goto tr621;
		case 66: goto tr622;
		case 67: goto tr623;
		case 70: goto tr624;
		case 72: goto tr625;
		case 73: goto tr626;
		case 77: goto tr627;
		case 78: goto tr628;
		case 80: goto tr629;
		case 82: goto tr630;
		case 83: goto tr631;
		case 84: goto tr632;
		case 85: goto tr633;
		case 87: goto tr634;
		case 91: goto tr635;
		case 92: goto st578;
		case 96: goto tr637;
		case 97: goto tr621;
		case 98: goto tr622;
		case 99: goto tr623;
		case 102: goto tr624;
		case 104: goto tr625;
		case 105: goto tr626;
		case 109: goto tr627;
		case 110: goto tr628;
		case 112: goto tr629;
		case 114: goto tr630;
		case 115: goto tr631;
		case 116: goto tr632;
		case 117: goto tr633;
		case 119: goto tr634;
		case 123: goto tr638;
	}
	goto tr616;
tr617:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
#line 467 "ext/dtext/dtext.cpp.rl"
	{( sm->act) = 81;}
	goto st527;
st527:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof527;
case 527:
#line 2867 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 10: goto tr97;
		case 13: goto st77;
		case 42: goto tr641;
		case 72: goto st92;
		case 91: goto st94;
		case 104: goto st92;
	}
	goto tr639;
tr97:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
#line 457 "ext/dtext/dtext.cpp.rl"
	{( sm->act) = 80;}
	goto st528;
st528:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof528;
case 528:
#line 2884 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 10: goto tr97;
		case 13: goto st77;
		case 91: goto st78;
	}
	goto tr644;
st77:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof77;
case 77:
	if ( (*( sm->p)) == 10 )
		goto tr97;
	goto tr96;
st78:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof78;
case 78:
	if ( (*( sm->p)) == 47 )
		goto st79;
	goto tr98;
st79:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof79;
case 79:
	switch( (*( sm->p)) ) {
		case 83: goto st80;
		case 84: goto st88;
		case 115: goto st80;
		case 116: goto st88;
	}
	goto tr98;
st80:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof80;
case 80:
	switch( (*( sm->p)) ) {
		case 80: goto st81;
		case 112: goto st81;
	}
	goto tr98;
st81:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof81;
case 81:
	switch( (*( sm->p)) ) {
		case 79: goto st82;
		case 111: goto st82;
	}
	goto tr96;
st82:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof82;
case 82:
	switch( (*( sm->p)) ) {
		case 73: goto st83;
		case 105: goto st83;
	}
	goto tr96;
st83:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof83;
case 83:
	switch( (*( sm->p)) ) {
		case 76: goto st84;
		case 108: goto st84;
	}
	goto tr96;
st84:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof84;
case 84:
	switch( (*( sm->p)) ) {
		case 69: goto st85;
		case 101: goto st85;
	}
	goto tr96;
st85:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof85;
case 85:
	switch( (*( sm->p)) ) {
		case 82: goto st86;
		case 114: goto st86;
	}
	goto tr96;
st86:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof86;
case 86:
	switch( (*( sm->p)) ) {
		case 83: goto st87;
		case 93: goto tr109;
		case 115: goto st87;
	}
	goto tr96;
st87:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof87;
case 87:
	if ( (*( sm->p)) == 93 )
		goto tr109;
	goto tr96;
st88:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof88;
case 88:
	switch( (*( sm->p)) ) {
		case 68: goto st89;
		case 100: goto st89;
	}
	goto tr96;
st89:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof89;
case 89:
	if ( (*( sm->p)) == 93 )
		goto tr111;
	goto tr96;
tr641:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st90;
st90:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof90;
case 90:
#line 3009 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 9: goto tr113;
		case 32: goto tr113;
		case 42: goto st90;
	}
	goto tr112;
tr113:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
	goto st91;
st91:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof91;
case 91:
#line 3022 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 9: goto tr116;
		case 10: goto tr112;
		case 13: goto tr112;
		case 32: goto tr116;
	}
	goto tr115;
tr115:
#line 94 "ext/dtext/dtext.cpp.rl"
	{ b1 = p; }
	goto st529;
st529:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof529;
case 529:
#line 3036 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 10: goto tr646;
		case 13: goto tr646;
	}
	goto st529;
tr116:
#line 94 "ext/dtext/dtext.cpp.rl"
	{ b1 = p; }
	goto st530;
st530:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof530;
case 530:
#line 3048 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 9: goto tr116;
		case 10: goto tr646;
		case 13: goto tr646;
		case 32: goto tr116;
	}
	goto tr115;
st92:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof92;
case 92:
	if ( 49 <= (*( sm->p)) && (*( sm->p)) <= 54 )
		goto tr117;
	goto tr112;
tr117:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st93;
st93:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof93;
case 93:
#line 3069 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 46 )
		goto tr118;
	goto tr112;
tr118:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
	goto st531;
st531:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof531;
case 531:
#line 3079 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 9: goto st531;
		case 32: goto st531;
	}
	goto tr648;
st94:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof94;
case 94:
	if ( (*( sm->p)) == 47 )
		goto st95;
	goto tr112;
st95:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof95;
case 95:
	switch( (*( sm->p)) ) {
		case 81: goto st96;
		case 83: goto st101;
		case 84: goto st88;
		case 113: goto st96;
		case 115: goto st101;
		case 116: goto st88;
	}
	goto tr112;
st96:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof96;
case 96:
	switch( (*( sm->p)) ) {
		case 85: goto st97;
		case 117: goto st97;
	}
	goto tr96;
st97:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof97;
case 97:
	switch( (*( sm->p)) ) {
		case 79: goto st98;
		case 111: goto st98;
	}
	goto tr96;
st98:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof98;
case 98:
	switch( (*( sm->p)) ) {
		case 84: goto st99;
		case 116: goto st99;
	}
	goto tr96;
st99:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof99;
case 99:
	switch( (*( sm->p)) ) {
		case 69: goto st100;
		case 101: goto st100;
	}
	goto tr96;
st100:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof100;
case 100:
	_widec = (*( sm->p));
	if ( 93 <= (*( sm->p)) && (*( sm->p)) <= 93 ) {
		_widec = (short)(128 + ((*( sm->p)) - -128));
		if ( 
#line 97 "ext/dtext/dtext.cpp.rl"
 dstack_is_open(BLOCK_QUOTE)  ) _widec += 256;
	}
	if ( _widec == 605 )
		goto st532;
	goto tr96;
st532:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof532;
case 532:
	switch( (*( sm->p)) ) {
		case 9: goto st532;
		case 32: goto st532;
	}
	goto tr650;
st101:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof101;
case 101:
	switch( (*( sm->p)) ) {
		case 69: goto st102;
		case 80: goto st81;
		case 101: goto st102;
		case 112: goto st81;
	}
	goto tr112;
st102:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof102;
case 102:
	switch( (*( sm->p)) ) {
		case 67: goto st103;
		case 99: goto st103;
	}
	goto tr96;
st103:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof103;
case 103:
	switch( (*( sm->p)) ) {
		case 84: goto st104;
		case 116: goto st104;
	}
	goto tr96;
st104:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof104;
case 104:
	switch( (*( sm->p)) ) {
		case 73: goto st105;
		case 105: goto st105;
	}
	goto tr96;
st105:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof105;
case 105:
	switch( (*( sm->p)) ) {
		case 79: goto st106;
		case 111: goto st106;
	}
	goto tr96;
st106:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof106;
case 106:
	switch( (*( sm->p)) ) {
		case 78: goto st107;
		case 110: goto st107;
	}
	goto tr96;
st107:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof107;
case 107:
	_widec = (*( sm->p));
	if ( 93 <= (*( sm->p)) && (*( sm->p)) <= 93 ) {
		_widec = (short)(640 + ((*( sm->p)) - -128));
		if ( 
#line 98 "ext/dtext/dtext.cpp.rl"
 dstack_is_open(BLOCK_SECTION)  ) _widec += 256;
	}
	if ( _widec == 1117 )
		goto st533;
	goto tr96;
st533:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof533;
case 533:
	switch( (*( sm->p)) ) {
		case 9: goto st533;
		case 32: goto st533;
	}
	goto tr651;
st534:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof534;
case 534:
	if ( (*( sm->p)) == 10 )
		goto tr617;
	goto tr652;
tr619:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st535;
st535:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof535;
case 535:
#line 3254 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 34 )
		goto tr653;
	goto tr654;
tr654:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st108;
st108:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof108;
case 108:
#line 3264 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 34 )
		goto tr136;
	goto st108;
tr136:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
	goto st109;
st109:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof109;
case 109:
#line 3274 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 58 )
		goto st110;
	goto tr134;
st110:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof110;
case 110:
	switch( (*( sm->p)) ) {
		case 35: goto tr138;
		case 47: goto tr138;
		case 72: goto tr139;
		case 91: goto st119;
		case 104: goto tr139;
	}
	goto tr134;
tr138:
#line 94 "ext/dtext/dtext.cpp.rl"
	{ b1 = p; }
	goto st111;
st111:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof111;
case 111:
#line 3296 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 32 )
		goto tr134;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto tr134;
	goto st536;
st536:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof536;
case 536:
	if ( (*( sm->p)) == 32 )
		goto tr655;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto tr655;
	goto st536;
tr139:
#line 94 "ext/dtext/dtext.cpp.rl"
	{ b1 = p; }
	goto st112;
st112:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof112;
case 112:
#line 3317 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 84: goto st113;
		case 116: goto st113;
	}
	goto tr134;
st113:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof113;
case 113:
	switch( (*( sm->p)) ) {
		case 84: goto st114;
		case 116: goto st114;
	}
	goto tr134;
st114:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof114;
case 114:
	switch( (*( sm->p)) ) {
		case 80: goto st115;
		case 112: goto st115;
	}
	goto tr134;
st115:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof115;
case 115:
	switch( (*( sm->p)) ) {
		case 58: goto st116;
		case 83: goto st118;
		case 115: goto st118;
	}
	goto tr134;
st116:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof116;
case 116:
	if ( (*( sm->p)) == 47 )
		goto st117;
	goto tr134;
st117:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof117;
case 117:
	if ( (*( sm->p)) == 47 )
		goto st111;
	goto tr134;
st118:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof118;
case 118:
	if ( (*( sm->p)) == 58 )
		goto st116;
	goto tr134;
st119:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof119;
case 119:
	switch( (*( sm->p)) ) {
		case 35: goto tr149;
		case 47: goto tr149;
		case 72: goto tr150;
		case 104: goto tr150;
	}
	goto tr134;
tr149:
#line 94 "ext/dtext/dtext.cpp.rl"
	{ b1 = p; }
	goto st120;
st120:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof120;
case 120:
#line 3389 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 32 )
		goto tr134;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto tr134;
	goto st121;
st121:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof121;
case 121:
	switch( (*( sm->p)) ) {
		case 32: goto tr134;
		case 93: goto tr152;
	}
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto tr134;
	goto st121;
tr150:
#line 94 "ext/dtext/dtext.cpp.rl"
	{ b1 = p; }
	goto st122;
st122:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof122;
case 122:
#line 3412 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 84: goto st123;
		case 116: goto st123;
	}
	goto tr134;
st123:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof123;
case 123:
	switch( (*( sm->p)) ) {
		case 84: goto st124;
		case 116: goto st124;
	}
	goto tr134;
st124:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof124;
case 124:
	switch( (*( sm->p)) ) {
		case 80: goto st125;
		case 112: goto st125;
	}
	goto tr134;
st125:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof125;
case 125:
	switch( (*( sm->p)) ) {
		case 58: goto st126;
		case 83: goto st128;
		case 115: goto st128;
	}
	goto tr134;
st126:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof126;
case 126:
	if ( (*( sm->p)) == 47 )
		goto st127;
	goto tr134;
st127:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof127;
case 127:
	if ( (*( sm->p)) == 47 )
		goto st120;
	goto tr134;
st128:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof128;
case 128:
	if ( (*( sm->p)) == 58 )
		goto st126;
	goto tr134;
tr620:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st537;
st537:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof537;
case 537:
#line 3473 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 72: goto tr656;
		case 104: goto tr656;
	}
	goto tr653;
tr656:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st129;
st129:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof129;
case 129:
#line 3485 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 84: goto st130;
		case 116: goto st130;
	}
	goto tr134;
st130:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof130;
case 130:
	switch( (*( sm->p)) ) {
		case 84: goto st131;
		case 116: goto st131;
	}
	goto tr134;
st131:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof131;
case 131:
	switch( (*( sm->p)) ) {
		case 80: goto st132;
		case 112: goto st132;
	}
	goto tr134;
st132:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof132;
case 132:
	switch( (*( sm->p)) ) {
		case 58: goto st133;
		case 83: goto st137;
		case 115: goto st137;
	}
	goto tr134;
st133:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof133;
case 133:
	if ( (*( sm->p)) == 47 )
		goto st134;
	goto tr134;
st134:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof134;
case 134:
	if ( (*( sm->p)) == 47 )
		goto st135;
	goto tr134;
st135:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof135;
case 135:
	if ( (*( sm->p)) == 32 )
		goto tr134;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto tr134;
	goto st136;
st136:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof136;
case 136:
	switch( (*( sm->p)) ) {
		case 32: goto tr134;
		case 62: goto tr168;
	}
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto tr134;
	goto st136;
st137:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof137;
case 137:
	if ( (*( sm->p)) == 58 )
		goto st133;
	goto tr134;
tr621:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st538;
st538:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof538;
case 538:
#line 3566 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 76: goto st138;
		case 82: goto st144;
		case 108: goto st138;
		case 114: goto st144;
	}
	goto tr653;
st138:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof138;
case 138:
	switch( (*( sm->p)) ) {
		case 73: goto st139;
		case 105: goto st139;
	}
	goto tr134;
st139:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof139;
case 139:
	switch( (*( sm->p)) ) {
		case 65: goto st140;
		case 97: goto st140;
	}
	goto tr134;
st140:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof140;
case 140:
	switch( (*( sm->p)) ) {
		case 83: goto st141;
		case 115: goto st141;
	}
	goto tr134;
st141:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof141;
case 141:
	if ( (*( sm->p)) == 32 )
		goto st142;
	goto tr134;
st142:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof142;
case 142:
	if ( (*( sm->p)) == 35 )
		goto st143;
	goto tr134;
st143:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof143;
case 143:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr174;
	goto tr134;
tr174:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st539;
st539:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof539;
case 539:
#line 3628 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st539;
	goto tr659;
st144:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof144;
case 144:
	switch( (*( sm->p)) ) {
		case 84: goto st145;
		case 116: goto st145;
	}
	goto tr134;
st145:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof145;
case 145:
	switch( (*( sm->p)) ) {
		case 73: goto st146;
		case 105: goto st146;
	}
	goto tr134;
st146:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof146;
case 146:
	switch( (*( sm->p)) ) {
		case 83: goto st147;
		case 115: goto st147;
	}
	goto tr134;
st147:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof147;
case 147:
	switch( (*( sm->p)) ) {
		case 84: goto st148;
		case 116: goto st148;
	}
	goto tr134;
st148:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof148;
case 148:
	if ( (*( sm->p)) == 32 )
		goto st149;
	goto tr134;
st149:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof149;
case 149:
	if ( (*( sm->p)) == 35 )
		goto st150;
	goto tr134;
st150:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof150;
case 150:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr181;
	goto tr134;
tr181:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st540;
st540:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof540;
case 540:
#line 3695 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st540;
	goto tr661;
tr622:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st541;
st541:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof541;
case 541:
#line 3705 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 65: goto st151;
		case 76: goto st155;
		case 85: goto st160;
		case 97: goto st151;
		case 108: goto st155;
		case 117: goto st160;
	}
	goto tr653;
st151:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof151;
case 151:
	switch( (*( sm->p)) ) {
		case 78: goto st152;
		case 110: goto st152;
	}
	goto tr134;
st152:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof152;
case 152:
	if ( (*( sm->p)) == 32 )
		goto st153;
	goto tr134;
st153:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof153;
case 153:
	if ( (*( sm->p)) == 35 )
		goto st154;
	goto tr134;
st154:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof154;
case 154:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr185;
	goto tr134;
tr185:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st542;
st542:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof542;
case 542:
#line 3751 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st542;
	goto tr666;
st155:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof155;
case 155:
	switch( (*( sm->p)) ) {
		case 73: goto st156;
		case 105: goto st156;
	}
	goto tr134;
st156:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof156;
case 156:
	switch( (*( sm->p)) ) {
		case 80: goto st157;
		case 112: goto st157;
	}
	goto tr134;
st157:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof157;
case 157:
	if ( (*( sm->p)) == 32 )
		goto st158;
	goto tr134;
st158:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof158;
case 158:
	if ( (*( sm->p)) == 35 )
		goto st159;
	goto tr134;
st159:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof159;
case 159:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr190;
	goto tr134;
tr190:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st543;
st543:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof543;
case 543:
#line 3800 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st543;
	goto tr668;
st160:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof160;
case 160:
	switch( (*( sm->p)) ) {
		case 82: goto st161;
		case 114: goto st161;
	}
	goto tr134;
st161:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof161;
case 161:
	if ( (*( sm->p)) == 32 )
		goto st162;
	goto tr134;
st162:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof162;
case 162:
	if ( (*( sm->p)) == 35 )
		goto st163;
	goto tr134;
st163:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof163;
case 163:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr194;
	goto tr134;
tr194:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st544;
st544:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof544;
case 544:
#line 3840 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st544;
	goto tr670;
tr623:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st545;
st545:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof545;
case 545:
#line 3850 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 79: goto st164;
		case 111: goto st164;
	}
	goto tr653;
st164:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof164;
case 164:
	switch( (*( sm->p)) ) {
		case 77: goto st165;
		case 109: goto st165;
	}
	goto tr134;
st165:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof165;
case 165:
	switch( (*( sm->p)) ) {
		case 77: goto st166;
		case 109: goto st166;
	}
	goto tr134;
st166:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof166;
case 166:
	switch( (*( sm->p)) ) {
		case 69: goto st167;
		case 101: goto st167;
	}
	goto tr134;
st167:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof167;
case 167:
	switch( (*( sm->p)) ) {
		case 78: goto st168;
		case 110: goto st168;
	}
	goto tr134;
st168:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof168;
case 168:
	switch( (*( sm->p)) ) {
		case 84: goto st169;
		case 116: goto st169;
	}
	goto tr134;
st169:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof169;
case 169:
	if ( (*( sm->p)) == 32 )
		goto st170;
	goto tr134;
st170:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof170;
case 170:
	if ( (*( sm->p)) == 35 )
		goto st171;
	goto tr134;
st171:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof171;
case 171:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr202;
	goto tr134;
tr202:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st546;
st546:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof546;
case 546:
#line 3928 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st546;
	goto tr673;
tr624:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st547;
st547:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof547;
case 547:
#line 3938 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 76: goto st172;
		case 79: goto st177;
		case 108: goto st172;
		case 111: goto st177;
	}
	goto tr653;
st172:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof172;
case 172:
	switch( (*( sm->p)) ) {
		case 65: goto st173;
		case 97: goto st173;
	}
	goto tr134;
st173:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof173;
case 173:
	switch( (*( sm->p)) ) {
		case 71: goto st174;
		case 103: goto st174;
	}
	goto tr134;
st174:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof174;
case 174:
	if ( (*( sm->p)) == 32 )
		goto st175;
	goto tr134;
st175:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof175;
case 175:
	if ( (*( sm->p)) == 35 )
		goto st176;
	goto tr134;
st176:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof176;
case 176:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr207;
	goto tr134;
tr207:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st548;
st548:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof548;
case 548:
#line 3991 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st548;
	goto tr677;
st177:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof177;
case 177:
	switch( (*( sm->p)) ) {
		case 82: goto st178;
		case 114: goto st178;
	}
	goto tr134;
st178:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof178;
case 178:
	switch( (*( sm->p)) ) {
		case 85: goto st179;
		case 117: goto st179;
	}
	goto tr134;
st179:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof179;
case 179:
	switch( (*( sm->p)) ) {
		case 77: goto st180;
		case 109: goto st180;
	}
	goto tr134;
st180:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof180;
case 180:
	if ( (*( sm->p)) == 32 )
		goto st181;
	goto tr134;
st181:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof181;
case 181:
	if ( (*( sm->p)) == 35 )
		goto st182;
	goto tr134;
st182:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof182;
case 182:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr213;
	goto tr134;
tr213:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st549;
st549:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof549;
case 549:
#line 4049 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st549;
	goto tr679;
tr625:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st550;
st550:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof550;
case 550:
#line 4059 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 84: goto st183;
		case 116: goto st183;
	}
	goto tr653;
st183:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof183;
case 183:
	switch( (*( sm->p)) ) {
		case 84: goto st184;
		case 116: goto st184;
	}
	goto tr134;
st184:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof184;
case 184:
	switch( (*( sm->p)) ) {
		case 80: goto st185;
		case 112: goto st185;
	}
	goto tr134;
st185:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof185;
case 185:
	switch( (*( sm->p)) ) {
		case 58: goto st186;
		case 83: goto st189;
		case 115: goto st189;
	}
	goto tr134;
st186:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof186;
case 186:
	if ( (*( sm->p)) == 47 )
		goto st187;
	goto tr134;
st187:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof187;
case 187:
	if ( (*( sm->p)) == 47 )
		goto st188;
	goto tr134;
st188:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof188;
case 188:
	if ( (*( sm->p)) == 32 )
		goto tr134;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto tr134;
	goto st551;
st551:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof551;
case 551:
	if ( (*( sm->p)) == 32 )
		goto tr682;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto tr682;
	goto st551;
st189:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof189;
case 189:
	if ( (*( sm->p)) == 58 )
		goto st186;
	goto tr134;
tr626:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st552;
st552:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof552;
case 552:
#line 4138 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 77: goto st190;
		case 109: goto st190;
	}
	goto tr653;
st190:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof190;
case 190:
	switch( (*( sm->p)) ) {
		case 80: goto st191;
		case 112: goto st191;
	}
	goto tr134;
st191:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof191;
case 191:
	switch( (*( sm->p)) ) {
		case 76: goto st192;
		case 108: goto st192;
	}
	goto tr134;
st192:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof192;
case 192:
	switch( (*( sm->p)) ) {
		case 73: goto st193;
		case 105: goto st193;
	}
	goto tr134;
st193:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof193;
case 193:
	switch( (*( sm->p)) ) {
		case 67: goto st194;
		case 99: goto st194;
	}
	goto tr134;
st194:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof194;
case 194:
	switch( (*( sm->p)) ) {
		case 65: goto st195;
		case 97: goto st195;
	}
	goto tr134;
st195:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof195;
case 195:
	switch( (*( sm->p)) ) {
		case 84: goto st196;
		case 116: goto st196;
	}
	goto tr134;
st196:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof196;
case 196:
	switch( (*( sm->p)) ) {
		case 73: goto st197;
		case 105: goto st197;
	}
	goto tr134;
st197:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof197;
case 197:
	switch( (*( sm->p)) ) {
		case 79: goto st198;
		case 111: goto st198;
	}
	goto tr134;
st198:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof198;
case 198:
	switch( (*( sm->p)) ) {
		case 78: goto st199;
		case 110: goto st199;
	}
	goto tr134;
st199:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof199;
case 199:
	if ( (*( sm->p)) == 32 )
		goto st200;
	goto tr134;
st200:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof200;
case 200:
	if ( (*( sm->p)) == 35 )
		goto st201;
	goto tr134;
st201:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof201;
case 201:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr232;
	goto tr134;
tr232:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st553;
st553:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof553;
case 553:
#line 4252 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st553;
	goto tr684;
tr627:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st554;
st554:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof554;
case 554:
#line 4262 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 79: goto st202;
		case 111: goto st202;
	}
	goto tr653;
st202:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof202;
case 202:
	switch( (*( sm->p)) ) {
		case 68: goto st203;
		case 100: goto st203;
	}
	goto tr134;
st203:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof203;
case 203:
	if ( (*( sm->p)) == 32 )
		goto st204;
	goto tr134;
st204:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof204;
case 204:
	switch( (*( sm->p)) ) {
		case 65: goto st205;
		case 97: goto st205;
	}
	goto tr134;
st205:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof205;
case 205:
	switch( (*( sm->p)) ) {
		case 67: goto st206;
		case 99: goto st206;
	}
	goto tr134;
st206:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof206;
case 206:
	switch( (*( sm->p)) ) {
		case 84: goto st207;
		case 116: goto st207;
	}
	goto tr134;
st207:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof207;
case 207:
	switch( (*( sm->p)) ) {
		case 73: goto st208;
		case 105: goto st208;
	}
	goto tr134;
st208:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof208;
case 208:
	switch( (*( sm->p)) ) {
		case 79: goto st209;
		case 111: goto st209;
	}
	goto tr134;
st209:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof209;
case 209:
	switch( (*( sm->p)) ) {
		case 78: goto st210;
		case 110: goto st210;
	}
	goto tr134;
st210:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof210;
case 210:
	if ( (*( sm->p)) == 32 )
		goto st211;
	goto tr134;
st211:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof211;
case 211:
	if ( (*( sm->p)) == 35 )
		goto st212;
	goto tr134;
st212:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof212;
case 212:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr243;
	goto tr134;
tr243:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st555;
st555:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof555;
case 555:
#line 4365 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st555;
	goto tr687;
tr628:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st556;
st556:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof556;
case 556:
#line 4375 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 79: goto st213;
		case 111: goto st213;
	}
	goto tr653;
st213:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof213;
case 213:
	switch( (*( sm->p)) ) {
		case 84: goto st214;
		case 116: goto st214;
	}
	goto tr134;
st214:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof214;
case 214:
	switch( (*( sm->p)) ) {
		case 69: goto st215;
		case 101: goto st215;
	}
	goto tr134;
st215:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof215;
case 215:
	if ( (*( sm->p)) == 32 )
		goto st216;
	goto tr134;
st216:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof216;
case 216:
	if ( (*( sm->p)) == 35 )
		goto st217;
	goto tr134;
st217:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof217;
case 217:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr248;
	goto tr134;
tr248:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st557;
st557:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof557;
case 557:
#line 4426 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st557;
	goto tr690;
tr629:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st558;
st558:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof558;
case 558:
#line 4436 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 79: goto st218;
		case 111: goto st218;
	}
	goto tr653;
st218:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof218;
case 218:
	switch( (*( sm->p)) ) {
		case 79: goto st219;
		case 83: goto st223;
		case 111: goto st219;
		case 115: goto st223;
	}
	goto tr134;
st219:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof219;
case 219:
	switch( (*( sm->p)) ) {
		case 76: goto st220;
		case 108: goto st220;
	}
	goto tr134;
st220:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof220;
case 220:
	if ( (*( sm->p)) == 32 )
		goto st221;
	goto tr134;
st221:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof221;
case 221:
	if ( (*( sm->p)) == 35 )
		goto st222;
	goto tr134;
st222:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof222;
case 222:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr254;
	goto tr134;
tr254:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st559;
st559:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof559;
case 559:
#line 4489 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st559;
	goto tr693;
st223:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof223;
case 223:
	switch( (*( sm->p)) ) {
		case 84: goto st224;
		case 116: goto st224;
	}
	goto tr134;
st224:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof224;
case 224:
	if ( (*( sm->p)) == 32 )
		goto st225;
	goto tr134;
st225:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof225;
case 225:
	switch( (*( sm->p)) ) {
		case 35: goto st226;
		case 67: goto st227;
		case 99: goto st227;
	}
	goto tr134;
st226:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof226;
case 226:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr259;
	goto tr134;
tr259:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st560;
st560:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof560;
case 560:
#line 4532 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st560;
	goto tr695;
st227:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof227;
case 227:
	switch( (*( sm->p)) ) {
		case 72: goto st228;
		case 104: goto st228;
	}
	goto tr134;
st228:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof228;
case 228:
	switch( (*( sm->p)) ) {
		case 65: goto st229;
		case 97: goto st229;
	}
	goto tr134;
st229:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof229;
case 229:
	switch( (*( sm->p)) ) {
		case 78: goto st230;
		case 110: goto st230;
	}
	goto tr134;
st230:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof230;
case 230:
	switch( (*( sm->p)) ) {
		case 71: goto st231;
		case 103: goto st231;
	}
	goto tr134;
st231:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof231;
case 231:
	switch( (*( sm->p)) ) {
		case 69: goto st232;
		case 101: goto st232;
	}
	goto tr134;
st232:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof232;
case 232:
	switch( (*( sm->p)) ) {
		case 83: goto st233;
		case 115: goto st233;
	}
	goto tr134;
st233:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof233;
case 233:
	if ( (*( sm->p)) == 32 )
		goto st234;
	goto tr134;
st234:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof234;
case 234:
	if ( (*( sm->p)) == 35 )
		goto st235;
	goto tr134;
st235:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof235;
case 235:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr268;
	goto tr134;
tr268:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st561;
st561:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof561;
case 561:
#line 4617 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st561;
	goto tr697;
tr630:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st562;
st562:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof562;
case 562:
#line 4627 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 69: goto st236;
		case 101: goto st236;
	}
	goto tr653;
st236:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof236;
case 236:
	switch( (*( sm->p)) ) {
		case 67: goto st237;
		case 99: goto st237;
	}
	goto tr134;
st237:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof237;
case 237:
	switch( (*( sm->p)) ) {
		case 79: goto st238;
		case 111: goto st238;
	}
	goto tr134;
st238:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof238;
case 238:
	switch( (*( sm->p)) ) {
		case 82: goto st239;
		case 114: goto st239;
	}
	goto tr134;
st239:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof239;
case 239:
	switch( (*( sm->p)) ) {
		case 68: goto st240;
		case 100: goto st240;
	}
	goto tr134;
st240:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof240;
case 240:
	if ( (*( sm->p)) == 32 )
		goto st241;
	goto tr134;
st241:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof241;
case 241:
	if ( (*( sm->p)) == 35 )
		goto st242;
	goto tr134;
st242:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof242;
case 242:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr275;
	goto tr134;
tr275:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st563;
st563:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof563;
case 563:
#line 4696 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st563;
	goto tr700;
tr631:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st564;
st564:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof564;
case 564:
#line 4706 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 69: goto st243;
		case 101: goto st243;
	}
	goto tr653;
st243:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof243;
case 243:
	switch( (*( sm->p)) ) {
		case 84: goto st244;
		case 116: goto st244;
	}
	goto tr134;
st244:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof244;
case 244:
	if ( (*( sm->p)) == 32 )
		goto st245;
	goto tr134;
st245:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof245;
case 245:
	if ( (*( sm->p)) == 35 )
		goto st246;
	goto tr134;
st246:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof246;
case 246:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr279;
	goto tr134;
tr279:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st565;
st565:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof565;
case 565:
#line 4748 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st565;
	goto tr703;
tr632:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st566;
st566:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof566;
case 566:
#line 4758 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 65: goto st247;
		case 72: goto st265;
		case 73: goto st271;
		case 79: goto st278;
		case 97: goto st247;
		case 104: goto st265;
		case 105: goto st271;
		case 111: goto st278;
	}
	goto tr653;
st247:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof247;
case 247:
	switch( (*( sm->p)) ) {
		case 75: goto st248;
		case 107: goto st248;
	}
	goto tr134;
st248:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof248;
case 248:
	switch( (*( sm->p)) ) {
		case 69: goto st249;
		case 101: goto st249;
	}
	goto tr134;
st249:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof249;
case 249:
	switch( (*( sm->p)) ) {
		case 32: goto st250;
		case 68: goto st251;
		case 100: goto st251;
	}
	goto tr134;
st250:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof250;
case 250:
	switch( (*( sm->p)) ) {
		case 68: goto st251;
		case 100: goto st251;
	}
	goto tr134;
st251:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof251;
case 251:
	switch( (*( sm->p)) ) {
		case 79: goto st252;
		case 111: goto st252;
	}
	goto tr134;
st252:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof252;
case 252:
	switch( (*( sm->p)) ) {
		case 87: goto st253;
		case 119: goto st253;
	}
	goto tr134;
st253:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof253;
case 253:
	switch( (*( sm->p)) ) {
		case 78: goto st254;
		case 110: goto st254;
	}
	goto tr134;
st254:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof254;
case 254:
	if ( (*( sm->p)) == 32 )
		goto st255;
	goto tr134;
st255:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof255;
case 255:
	switch( (*( sm->p)) ) {
		case 35: goto st256;
		case 82: goto st257;
		case 114: goto st257;
	}
	goto tr134;
st256:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof256;
case 256:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr290;
	goto tr134;
tr290:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st567;
st567:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof567;
case 567:
#line 4864 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st567;
	goto tr709;
st257:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof257;
case 257:
	switch( (*( sm->p)) ) {
		case 69: goto st258;
		case 101: goto st258;
	}
	goto tr134;
st258:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof258;
case 258:
	switch( (*( sm->p)) ) {
		case 81: goto st259;
		case 113: goto st259;
	}
	goto tr134;
st259:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof259;
case 259:
	switch( (*( sm->p)) ) {
		case 85: goto st260;
		case 117: goto st260;
	}
	goto tr134;
st260:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof260;
case 260:
	switch( (*( sm->p)) ) {
		case 69: goto st261;
		case 101: goto st261;
	}
	goto tr134;
st261:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof261;
case 261:
	switch( (*( sm->p)) ) {
		case 83: goto st262;
		case 115: goto st262;
	}
	goto tr134;
st262:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof262;
case 262:
	switch( (*( sm->p)) ) {
		case 84: goto st263;
		case 116: goto st263;
	}
	goto tr134;
st263:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof263;
case 263:
	if ( (*( sm->p)) == 32 )
		goto st264;
	goto tr134;
st264:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof264;
case 264:
	if ( (*( sm->p)) == 35 )
		goto st256;
	goto tr134;
st265:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof265;
case 265:
	switch( (*( sm->p)) ) {
		case 85: goto st266;
		case 117: goto st266;
	}
	goto tr134;
st266:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof266;
case 266:
	switch( (*( sm->p)) ) {
		case 77: goto st267;
		case 109: goto st267;
	}
	goto tr134;
st267:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof267;
case 267:
	switch( (*( sm->p)) ) {
		case 66: goto st268;
		case 98: goto st268;
	}
	goto tr134;
st268:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof268;
case 268:
	if ( (*( sm->p)) == 32 )
		goto st269;
	goto tr134;
st269:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof269;
case 269:
	if ( (*( sm->p)) == 35 )
		goto st270;
	goto tr134;
st270:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof270;
case 270:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr303;
	goto tr134;
tr303:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st568;
st568:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof568;
case 568:
#line 4990 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st568;
	goto tr711;
st271:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof271;
case 271:
	switch( (*( sm->p)) ) {
		case 67: goto st272;
		case 99: goto st272;
	}
	goto tr134;
st272:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof272;
case 272:
	switch( (*( sm->p)) ) {
		case 75: goto st273;
		case 107: goto st273;
	}
	goto tr134;
st273:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof273;
case 273:
	switch( (*( sm->p)) ) {
		case 69: goto st274;
		case 101: goto st274;
	}
	goto tr134;
st274:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof274;
case 274:
	switch( (*( sm->p)) ) {
		case 84: goto st275;
		case 116: goto st275;
	}
	goto tr134;
st275:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof275;
case 275:
	if ( (*( sm->p)) == 32 )
		goto st276;
	goto tr134;
st276:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof276;
case 276:
	if ( (*( sm->p)) == 35 )
		goto st277;
	goto tr134;
st277:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof277;
case 277:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr310;
	goto tr134;
tr310:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st569;
st569:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof569;
case 569:
#line 5057 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st569;
	goto tr713;
st278:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof278;
case 278:
	switch( (*( sm->p)) ) {
		case 80: goto st279;
		case 112: goto st279;
	}
	goto tr134;
st279:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof279;
case 279:
	switch( (*( sm->p)) ) {
		case 73: goto st280;
		case 105: goto st280;
	}
	goto tr134;
st280:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof280;
case 280:
	switch( (*( sm->p)) ) {
		case 67: goto st281;
		case 99: goto st281;
	}
	goto tr134;
st281:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof281;
case 281:
	if ( (*( sm->p)) == 32 )
		goto st282;
	goto tr134;
st282:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof282;
case 282:
	if ( (*( sm->p)) == 35 )
		goto st283;
	goto tr134;
st283:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof283;
case 283:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr316;
	goto tr134;
tr316:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st570;
st570:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof570;
case 570:
#line 5115 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st570;
	goto tr715;
tr633:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st571;
st571:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof571;
case 571:
#line 5125 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 83: goto st284;
		case 115: goto st284;
	}
	goto tr653;
st284:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof284;
case 284:
	switch( (*( sm->p)) ) {
		case 69: goto st285;
		case 101: goto st285;
	}
	goto tr134;
st285:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof285;
case 285:
	switch( (*( sm->p)) ) {
		case 82: goto st286;
		case 114: goto st286;
	}
	goto tr134;
st286:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof286;
case 286:
	if ( (*( sm->p)) == 32 )
		goto st287;
	goto tr134;
st287:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof287;
case 287:
	if ( (*( sm->p)) == 35 )
		goto st288;
	goto tr134;
st288:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof288;
case 288:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr321;
	goto tr134;
tr321:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st572;
st572:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof572;
case 572:
#line 5176 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st572;
	goto tr718;
tr634:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st573;
st573:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof573;
case 573:
#line 5186 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 73: goto st289;
		case 105: goto st289;
	}
	goto tr653;
st289:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof289;
case 289:
	switch( (*( sm->p)) ) {
		case 75: goto st290;
		case 107: goto st290;
	}
	goto tr134;
st290:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof290;
case 290:
	switch( (*( sm->p)) ) {
		case 73: goto st291;
		case 105: goto st291;
	}
	goto tr134;
st291:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof291;
case 291:
	if ( (*( sm->p)) == 32 )
		goto st292;
	goto tr134;
st292:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof292;
case 292:
	if ( (*( sm->p)) == 35 )
		goto st293;
	goto tr134;
st293:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof293;
case 293:
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto tr326;
	goto tr134;
tr326:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st574;
st574:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof574;
case 574:
#line 5237 "ext/dtext/dtext.cpp"
	if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
		goto st574;
	goto tr721;
tr635:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
#line 485 "ext/dtext/dtext.cpp.rl"
	{( sm->act) = 83;}
	goto st575;
st575:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof575;
case 575:
#line 5248 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 35: goto st294;
		case 47: goto st296;
		case 66: goto st321;
		case 67: goto st322;
		case 73: goto st418;
		case 81: goto st419;
		case 83: goto st424;
		case 84: goto st452;
		case 85: goto st457;
		case 86: goto st458;
		case 91: goto st462;
		case 98: goto st321;
		case 99: goto st322;
		case 105: goto st418;
		case 113: goto st419;
		case 115: goto st424;
		case 116: goto st452;
		case 117: goto st457;
		case 118: goto st458;
	}
	goto tr653;
st294:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof294;
case 294:
	switch( (*( sm->p)) ) {
		case 45: goto tr327;
		case 95: goto tr327;
	}
	if ( (*( sm->p)) < 65 ) {
		if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
			goto tr327;
	} else if ( (*( sm->p)) > 90 ) {
		if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
			goto tr327;
	} else
		goto tr327;
	goto tr134;
tr327:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st295;
st295:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof295;
case 295:
#line 5294 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 45: goto st295;
		case 93: goto tr329;
		case 95: goto st295;
	}
	if ( (*( sm->p)) < 65 ) {
		if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
			goto st295;
	} else if ( (*( sm->p)) > 90 ) {
		if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
			goto st295;
	} else
		goto st295;
	goto tr134;
st296:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof296;
case 296:
	switch( (*( sm->p)) ) {
		case 66: goto st297;
		case 67: goto st298;
		case 73: goto st305;
		case 81: goto st96;
		case 83: goto st306;
		case 84: goto st310;
		case 85: goto st316;
		case 86: goto st317;
		case 98: goto st297;
		case 99: goto st298;
		case 105: goto st305;
		case 113: goto st96;
		case 115: goto st306;
		case 116: goto st310;
		case 117: goto st316;
		case 118: goto st317;
	}
	goto tr134;
st297:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof297;
case 297:
	if ( (*( sm->p)) == 93 )
		goto tr337;
	goto tr134;
st298:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof298;
case 298:
	switch( (*( sm->p)) ) {
		case 79: goto st299;
		case 111: goto st299;
	}
	goto tr134;
st299:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof299;
case 299:
	switch( (*( sm->p)) ) {
		case 68: goto st300;
		case 76: goto st302;
		case 100: goto st300;
		case 108: goto st302;
	}
	goto tr134;
st300:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof300;
case 300:
	switch( (*( sm->p)) ) {
		case 69: goto st301;
		case 101: goto st301;
	}
	goto tr134;
st301:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof301;
case 301:
	if ( (*( sm->p)) == 93 )
		goto st576;
	goto tr134;
st576:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof576;
case 576:
	if ( (*( sm->p)) == 32 )
		goto st576;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto st576;
	goto tr734;
st302:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof302;
case 302:
	switch( (*( sm->p)) ) {
		case 79: goto st303;
		case 111: goto st303;
	}
	goto tr134;
st303:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof303;
case 303:
	switch( (*( sm->p)) ) {
		case 82: goto st304;
		case 114: goto st304;
	}
	goto tr134;
st304:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof304;
case 304:
	if ( (*( sm->p)) == 93 )
		goto tr345;
	goto tr134;
st305:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof305;
case 305:
	if ( (*( sm->p)) == 93 )
		goto tr346;
	goto tr134;
st306:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof306;
case 306:
	switch( (*( sm->p)) ) {
		case 69: goto st102;
		case 80: goto st81;
		case 85: goto st307;
		case 93: goto tr348;
		case 101: goto st102;
		case 112: goto st81;
		case 117: goto st307;
	}
	goto tr134;
st307:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof307;
case 307:
	switch( (*( sm->p)) ) {
		case 66: goto st308;
		case 80: goto st309;
		case 98: goto st308;
		case 112: goto st309;
	}
	goto tr134;
st308:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof308;
case 308:
	if ( (*( sm->p)) == 93 )
		goto tr351;
	goto tr134;
st309:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof309;
case 309:
	if ( (*( sm->p)) == 93 )
		goto tr352;
	goto tr134;
st310:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof310;
case 310:
	switch( (*( sm->p)) ) {
		case 65: goto st311;
		case 68: goto st89;
		case 72: goto st315;
		case 97: goto st311;
		case 100: goto st89;
		case 104: goto st315;
	}
	goto tr134;
st311:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof311;
case 311:
	switch( (*( sm->p)) ) {
		case 66: goto st312;
		case 98: goto st312;
	}
	goto tr134;
st312:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof312;
case 312:
	switch( (*( sm->p)) ) {
		case 76: goto st313;
		case 108: goto st313;
	}
	goto tr134;
st313:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof313;
case 313:
	switch( (*( sm->p)) ) {
		case 69: goto st314;
		case 101: goto st314;
	}
	goto tr134;
st314:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof314;
case 314:
	if ( (*( sm->p)) == 93 )
		goto st577;
	goto tr134;
st577:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof577;
case 577:
	if ( (*( sm->p)) == 32 )
		goto st577;
	if ( 9 <= (*( sm->p)) && (*( sm->p)) <= 13 )
		goto st577;
	goto tr735;
st315:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof315;
case 315:
	if ( (*( sm->p)) == 93 )
		goto tr359;
	goto tr134;
st316:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof316;
case 316:
	if ( (*( sm->p)) == 93 )
		goto tr360;
	goto tr134;
st317:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof317;
case 317:
	switch( (*( sm->p)) ) {
		case 79: goto st318;
		case 111: goto st318;
	}
	goto tr134;
st318:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof318;
case 318:
	switch( (*( sm->p)) ) {
		case 73: goto st319;
		case 105: goto st319;
	}
	goto tr134;
st319:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof319;
case 319:
	switch( (*( sm->p)) ) {
		case 68: goto st320;
		case 100: goto st320;
	}
	goto tr134;
st320:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof320;
case 320:
	if ( (*( sm->p)) == 93 )
		goto tr364;
	goto tr134;
st321:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof321;
case 321:
	if ( (*( sm->p)) == 93 )
		goto tr365;
	goto tr134;
st322:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof322;
case 322:
	switch( (*( sm->p)) ) {
		case 79: goto st323;
		case 111: goto st323;
	}
	goto tr134;
st323:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof323;
case 323:
	switch( (*( sm->p)) ) {
		case 68: goto st324;
		case 76: goto st326;
		case 100: goto st324;
		case 108: goto st326;
	}
	goto tr134;
st324:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof324;
case 324:
	switch( (*( sm->p)) ) {
		case 69: goto st325;
		case 101: goto st325;
	}
	goto tr134;
st325:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof325;
case 325:
	if ( (*( sm->p)) == 93 )
		goto tr370;
	goto tr134;
st326:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof326;
case 326:
	switch( (*( sm->p)) ) {
		case 79: goto st327;
		case 111: goto st327;
	}
	goto tr134;
st327:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof327;
case 327:
	switch( (*( sm->p)) ) {
		case 82: goto st328;
		case 114: goto st328;
	}
	goto tr134;
st328:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof328;
case 328:
	if ( (*( sm->p)) == 61 )
		goto st329;
	goto tr134;
st329:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof329;
case 329:
	switch( (*( sm->p)) ) {
		case 35: goto tr374;
		case 65: goto tr375;
		case 67: goto tr376;
		case 73: goto tr377;
		case 76: goto tr378;
		case 77: goto tr379;
		case 83: goto tr380;
		case 97: goto tr381;
		case 99: goto tr383;
		case 105: goto tr384;
		case 108: goto tr385;
		case 109: goto tr386;
		case 115: goto tr387;
	}
	if ( 98 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto tr382;
	goto tr134;
tr374:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st330;
st330:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof330;
case 330:
#line 5655 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) < 65 ) {
		if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
			goto st331;
	} else if ( (*( sm->p)) > 70 ) {
		if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 102 )
			goto st331;
	} else
		goto st331;
	goto tr134;
st331:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof331;
case 331:
	if ( (*( sm->p)) < 65 ) {
		if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
			goto st332;
	} else if ( (*( sm->p)) > 70 ) {
		if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 102 )
			goto st332;
	} else
		goto st332;
	goto tr134;
st332:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof332;
case 332:
	if ( (*( sm->p)) < 65 ) {
		if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
			goto st333;
	} else if ( (*( sm->p)) > 70 ) {
		if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 102 )
			goto st333;
	} else
		goto st333;
	goto tr134;
st333:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof333;
case 333:
	if ( (*( sm->p)) == 93 )
		goto tr392;
	if ( (*( sm->p)) < 65 ) {
		if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
			goto st334;
	} else if ( (*( sm->p)) > 70 ) {
		if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 102 )
			goto st334;
	} else
		goto st334;
	goto tr134;
st334:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof334;
case 334:
	if ( (*( sm->p)) == 93 )
		goto tr392;
	if ( (*( sm->p)) < 65 ) {
		if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
			goto st335;
	} else if ( (*( sm->p)) > 70 ) {
		if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 102 )
			goto st335;
	} else
		goto st335;
	goto tr134;
st335:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof335;
case 335:
	if ( (*( sm->p)) == 93 )
		goto tr392;
	if ( (*( sm->p)) < 65 ) {
		if ( 48 <= (*( sm->p)) && (*( sm->p)) <= 57 )
			goto st336;
	} else if ( (*( sm->p)) > 70 ) {
		if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 102 )
			goto st336;
	} else
		goto st336;
	goto tr134;
st336:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof336;
case 336:
	if ( (*( sm->p)) == 93 )
		goto tr392;
	goto tr134;
tr375:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st337;
st337:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof337;
case 337:
#line 5749 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 82: goto st338;
		case 114: goto st338;
	}
	goto tr134;
st338:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof338;
case 338:
	switch( (*( sm->p)) ) {
		case 84: goto st339;
		case 116: goto st339;
	}
	goto tr134;
st339:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof339;
case 339:
	switch( (*( sm->p)) ) {
		case 73: goto st340;
		case 93: goto tr398;
		case 105: goto st340;
	}
	goto tr134;
st340:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof340;
case 340:
	switch( (*( sm->p)) ) {
		case 83: goto st341;
		case 115: goto st341;
	}
	goto tr134;
st341:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof341;
case 341:
	switch( (*( sm->p)) ) {
		case 84: goto st342;
		case 116: goto st342;
	}
	goto tr134;
st342:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof342;
case 342:
	if ( (*( sm->p)) == 93 )
		goto tr398;
	goto tr134;
tr376:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st343;
st343:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof343;
case 343:
#line 5805 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 72: goto st344;
		case 76: goto st351;
		case 79: goto st354;
		case 104: goto st344;
		case 108: goto st351;
		case 111: goto st354;
	}
	goto tr134;
st344:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof344;
case 344:
	switch( (*( sm->p)) ) {
		case 65: goto st345;
		case 97: goto st345;
	}
	goto tr134;
st345:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof345;
case 345:
	switch( (*( sm->p)) ) {
		case 82: goto st346;
		case 114: goto st346;
	}
	goto tr134;
st346:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof346;
case 346:
	switch( (*( sm->p)) ) {
		case 65: goto st347;
		case 93: goto tr398;
		case 97: goto st347;
	}
	goto tr134;
st347:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof347;
case 347:
	switch( (*( sm->p)) ) {
		case 67: goto st348;
		case 99: goto st348;
	}
	goto tr134;
st348:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof348;
case 348:
	switch( (*( sm->p)) ) {
		case 84: goto st349;
		case 116: goto st349;
	}
	goto tr134;
st349:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof349;
case 349:
	switch( (*( sm->p)) ) {
		case 69: goto st350;
		case 101: goto st350;
	}
	goto tr134;
st350:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof350;
case 350:
	switch( (*( sm->p)) ) {
		case 82: goto st342;
		case 114: goto st342;
	}
	goto tr134;
st351:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof351;
case 351:
	switch( (*( sm->p)) ) {
		case 65: goto st352;
		case 93: goto tr398;
		case 97: goto st352;
	}
	goto tr134;
st352:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof352;
case 352:
	switch( (*( sm->p)) ) {
		case 83: goto st353;
		case 115: goto st353;
	}
	goto tr134;
st353:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof353;
case 353:
	switch( (*( sm->p)) ) {
		case 83: goto st342;
		case 115: goto st342;
	}
	goto tr134;
st354:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof354;
case 354:
	switch( (*( sm->p)) ) {
		case 80: goto st355;
		case 112: goto st355;
	}
	goto tr134;
st355:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof355;
case 355:
	switch( (*( sm->p)) ) {
		case 89: goto st356;
		case 121: goto st356;
	}
	goto tr134;
st356:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof356;
case 356:
	switch( (*( sm->p)) ) {
		case 82: goto st357;
		case 93: goto tr398;
		case 114: goto st357;
	}
	goto tr134;
st357:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof357;
case 357:
	switch( (*( sm->p)) ) {
		case 73: goto st358;
		case 105: goto st358;
	}
	goto tr134;
st358:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof358;
case 358:
	switch( (*( sm->p)) ) {
		case 71: goto st359;
		case 103: goto st359;
	}
	goto tr134;
st359:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof359;
case 359:
	switch( (*( sm->p)) ) {
		case 72: goto st341;
		case 104: goto st341;
	}
	goto tr134;
tr377:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st360;
st360:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof360;
case 360:
#line 5968 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 78: goto st361;
		case 110: goto st361;
	}
	goto tr134;
st361:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof361;
case 361:
	switch( (*( sm->p)) ) {
		case 86: goto st362;
		case 118: goto st362;
	}
	goto tr134;
st362:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof362;
case 362:
	switch( (*( sm->p)) ) {
		case 65: goto st363;
		case 93: goto tr398;
		case 97: goto st363;
	}
	goto tr134;
st363:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof363;
case 363:
	switch( (*( sm->p)) ) {
		case 76: goto st364;
		case 108: goto st364;
	}
	goto tr134;
st364:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof364;
case 364:
	switch( (*( sm->p)) ) {
		case 73: goto st365;
		case 105: goto st365;
	}
	goto tr134;
st365:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof365;
case 365:
	switch( (*( sm->p)) ) {
		case 68: goto st342;
		case 100: goto st342;
	}
	goto tr134;
tr378:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st366;
st366:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof366;
case 366:
#line 6026 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 79: goto st367;
		case 111: goto st367;
	}
	goto tr134;
st367:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof367;
case 367:
	switch( (*( sm->p)) ) {
		case 82: goto st368;
		case 114: goto st368;
	}
	goto tr134;
st368:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof368;
case 368:
	switch( (*( sm->p)) ) {
		case 69: goto st342;
		case 101: goto st342;
	}
	goto tr134;
tr379:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st369;
st369:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof369;
case 369:
#line 6056 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 69: goto st370;
		case 101: goto st370;
	}
	goto tr134;
st370:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof370;
case 370:
	switch( (*( sm->p)) ) {
		case 84: goto st371;
		case 116: goto st371;
	}
	goto tr134;
st371:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof371;
case 371:
	switch( (*( sm->p)) ) {
		case 65: goto st342;
		case 97: goto st342;
	}
	goto tr134;
tr380:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st372;
st372:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof372;
case 372:
#line 6086 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 80: goto st373;
		case 112: goto st373;
	}
	goto tr134;
st373:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof373;
case 373:
	switch( (*( sm->p)) ) {
		case 69: goto st374;
		case 101: goto st374;
	}
	goto tr134;
st374:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof374;
case 374:
	switch( (*( sm->p)) ) {
		case 67: goto st375;
		case 99: goto st375;
	}
	goto tr134;
st375:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof375;
case 375:
	switch( (*( sm->p)) ) {
		case 73: goto st376;
		case 93: goto tr398;
		case 105: goto st376;
	}
	goto tr134;
st376:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof376;
case 376:
	switch( (*( sm->p)) ) {
		case 69: goto st353;
		case 101: goto st353;
	}
	goto tr134;
tr381:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st377;
st377:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof377;
case 377:
#line 6135 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 82: goto st338;
		case 93: goto tr392;
		case 114: goto st379;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
tr382:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st378;
st378:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof378;
case 378:
#line 6150 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 93 )
		goto tr392;
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st379:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof379;
case 379:
	switch( (*( sm->p)) ) {
		case 84: goto st339;
		case 93: goto tr392;
		case 116: goto st380;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st380:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof380;
case 380:
	switch( (*( sm->p)) ) {
		case 73: goto st340;
		case 93: goto tr398;
		case 105: goto st381;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st381:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof381;
case 381:
	switch( (*( sm->p)) ) {
		case 83: goto st341;
		case 93: goto tr392;
		case 115: goto st382;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st382:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof382;
case 382:
	switch( (*( sm->p)) ) {
		case 84: goto st342;
		case 93: goto tr392;
		case 116: goto st383;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st383:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof383;
case 383:
	if ( (*( sm->p)) == 93 )
		goto tr398;
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
tr383:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st384;
st384:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof384;
case 384:
#line 6219 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 72: goto st344;
		case 76: goto st351;
		case 79: goto st354;
		case 93: goto tr392;
		case 104: goto st385;
		case 108: goto st392;
		case 111: goto st395;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st385:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof385;
case 385:
	switch( (*( sm->p)) ) {
		case 65: goto st345;
		case 93: goto tr392;
		case 97: goto st386;
	}
	if ( 98 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st386:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof386;
case 386:
	switch( (*( sm->p)) ) {
		case 82: goto st346;
		case 93: goto tr392;
		case 114: goto st387;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st387:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof387;
case 387:
	switch( (*( sm->p)) ) {
		case 65: goto st347;
		case 93: goto tr398;
		case 97: goto st388;
	}
	if ( 98 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st388:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof388;
case 388:
	switch( (*( sm->p)) ) {
		case 67: goto st348;
		case 93: goto tr392;
		case 99: goto st389;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st389:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof389;
case 389:
	switch( (*( sm->p)) ) {
		case 84: goto st349;
		case 93: goto tr392;
		case 116: goto st390;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st390:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof390;
case 390:
	switch( (*( sm->p)) ) {
		case 69: goto st350;
		case 93: goto tr392;
		case 101: goto st391;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st391:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof391;
case 391:
	switch( (*( sm->p)) ) {
		case 82: goto st342;
		case 93: goto tr392;
		case 114: goto st383;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st392:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof392;
case 392:
	switch( (*( sm->p)) ) {
		case 65: goto st352;
		case 93: goto tr398;
		case 97: goto st393;
	}
	if ( 98 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st393:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof393;
case 393:
	switch( (*( sm->p)) ) {
		case 83: goto st353;
		case 93: goto tr392;
		case 115: goto st394;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st394:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof394;
case 394:
	switch( (*( sm->p)) ) {
		case 83: goto st342;
		case 93: goto tr392;
		case 115: goto st383;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st395:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof395;
case 395:
	switch( (*( sm->p)) ) {
		case 80: goto st355;
		case 93: goto tr392;
		case 112: goto st396;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st396:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof396;
case 396:
	switch( (*( sm->p)) ) {
		case 89: goto st356;
		case 93: goto tr392;
		case 121: goto st397;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st397:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof397;
case 397:
	switch( (*( sm->p)) ) {
		case 82: goto st357;
		case 93: goto tr398;
		case 114: goto st398;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st398:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof398;
case 398:
	switch( (*( sm->p)) ) {
		case 73: goto st358;
		case 93: goto tr392;
		case 105: goto st399;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st399:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof399;
case 399:
	switch( (*( sm->p)) ) {
		case 71: goto st359;
		case 93: goto tr392;
		case 103: goto st400;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st400:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof400;
case 400:
	switch( (*( sm->p)) ) {
		case 72: goto st341;
		case 93: goto tr392;
		case 104: goto st382;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
tr384:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st401;
st401:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof401;
case 401:
#line 6430 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 78: goto st361;
		case 93: goto tr392;
		case 110: goto st402;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st402:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof402;
case 402:
	switch( (*( sm->p)) ) {
		case 86: goto st362;
		case 93: goto tr392;
		case 118: goto st403;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st403:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof403;
case 403:
	switch( (*( sm->p)) ) {
		case 65: goto st363;
		case 93: goto tr398;
		case 97: goto st404;
	}
	if ( 98 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st404:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof404;
case 404:
	switch( (*( sm->p)) ) {
		case 76: goto st364;
		case 93: goto tr392;
		case 108: goto st405;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st405:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof405;
case 405:
	switch( (*( sm->p)) ) {
		case 73: goto st365;
		case 93: goto tr392;
		case 105: goto st406;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st406:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof406;
case 406:
	switch( (*( sm->p)) ) {
		case 68: goto st342;
		case 93: goto tr392;
		case 100: goto st383;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
tr385:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st407;
st407:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof407;
case 407:
#line 6505 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 79: goto st367;
		case 93: goto tr392;
		case 111: goto st408;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st408:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof408;
case 408:
	switch( (*( sm->p)) ) {
		case 82: goto st368;
		case 93: goto tr392;
		case 114: goto st409;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st409:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof409;
case 409:
	switch( (*( sm->p)) ) {
		case 69: goto st342;
		case 93: goto tr392;
		case 101: goto st383;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
tr386:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st410;
st410:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof410;
case 410:
#line 6544 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 69: goto st370;
		case 93: goto tr392;
		case 101: goto st411;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st411:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof411;
case 411:
	switch( (*( sm->p)) ) {
		case 84: goto st371;
		case 93: goto tr392;
		case 116: goto st412;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st412:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof412;
case 412:
	switch( (*( sm->p)) ) {
		case 65: goto st342;
		case 93: goto tr392;
		case 97: goto st383;
	}
	if ( 98 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
tr387:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st413;
st413:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof413;
case 413:
#line 6583 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 80: goto st373;
		case 93: goto tr392;
		case 112: goto st414;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st414:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof414;
case 414:
	switch( (*( sm->p)) ) {
		case 69: goto st374;
		case 93: goto tr392;
		case 101: goto st415;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st415:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof415;
case 415:
	switch( (*( sm->p)) ) {
		case 67: goto st375;
		case 93: goto tr392;
		case 99: goto st416;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st416:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof416;
case 416:
	switch( (*( sm->p)) ) {
		case 73: goto st376;
		case 93: goto tr398;
		case 105: goto st417;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st417:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof417;
case 417:
	switch( (*( sm->p)) ) {
		case 69: goto st353;
		case 93: goto tr392;
		case 101: goto st394;
	}
	if ( 97 <= (*( sm->p)) && (*( sm->p)) <= 122 )
		goto st378;
	goto tr134;
st418:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof418;
case 418:
	if ( (*( sm->p)) == 93 )
		goto tr465;
	goto tr134;
st419:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof419;
case 419:
	switch( (*( sm->p)) ) {
		case 85: goto st420;
		case 117: goto st420;
	}
	goto tr134;
st420:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof420;
case 420:
	switch( (*( sm->p)) ) {
		case 79: goto st421;
		case 111: goto st421;
	}
	goto tr134;
st421:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof421;
case 421:
	switch( (*( sm->p)) ) {
		case 84: goto st422;
		case 116: goto st422;
	}
	goto tr134;
st422:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof422;
case 422:
	switch( (*( sm->p)) ) {
		case 69: goto st423;
		case 101: goto st423;
	}
	goto tr134;
st423:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof423;
case 423:
	if ( (*( sm->p)) == 93 )
		goto tr470;
	goto tr134;
st424:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof424;
case 424:
	switch( (*( sm->p)) ) {
		case 69: goto st425;
		case 80: goto st442;
		case 85: goto st449;
		case 93: goto tr474;
		case 101: goto st425;
		case 112: goto st442;
		case 117: goto st449;
	}
	goto tr134;
st425:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof425;
case 425:
	switch( (*( sm->p)) ) {
		case 67: goto st426;
		case 99: goto st426;
	}
	goto tr134;
st426:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof426;
case 426:
	switch( (*( sm->p)) ) {
		case 84: goto st427;
		case 116: goto st427;
	}
	goto tr134;
st427:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof427;
case 427:
	switch( (*( sm->p)) ) {
		case 73: goto st428;
		case 105: goto st428;
	}
	goto tr134;
st428:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof428;
case 428:
	switch( (*( sm->p)) ) {
		case 79: goto st429;
		case 111: goto st429;
	}
	goto tr134;
st429:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof429;
case 429:
	switch( (*( sm->p)) ) {
		case 78: goto st430;
		case 110: goto st430;
	}
	goto tr134;
st430:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof430;
case 430:
	switch( (*( sm->p)) ) {
		case 44: goto st431;
		case 61: goto st440;
		case 93: goto tr482;
	}
	goto tr134;
st431:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof431;
case 431:
	switch( (*( sm->p)) ) {
		case 69: goto st432;
		case 101: goto st432;
	}
	goto tr134;
st432:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof432;
case 432:
	switch( (*( sm->p)) ) {
		case 88: goto st433;
		case 120: goto st433;
	}
	goto tr134;
st433:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof433;
case 433:
	switch( (*( sm->p)) ) {
		case 80: goto st434;
		case 112: goto st434;
	}
	goto tr134;
st434:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof434;
case 434:
	switch( (*( sm->p)) ) {
		case 65: goto st435;
		case 97: goto st435;
	}
	goto tr134;
st435:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof435;
case 435:
	switch( (*( sm->p)) ) {
		case 78: goto st436;
		case 110: goto st436;
	}
	goto tr134;
st436:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof436;
case 436:
	switch( (*( sm->p)) ) {
		case 68: goto st437;
		case 100: goto st437;
	}
	goto tr134;
st437:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof437;
case 437:
	switch( (*( sm->p)) ) {
		case 69: goto st438;
		case 101: goto st438;
	}
	goto tr134;
st438:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof438;
case 438:
	switch( (*( sm->p)) ) {
		case 68: goto st439;
		case 100: goto st439;
	}
	goto tr134;
st439:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof439;
case 439:
	switch( (*( sm->p)) ) {
		case 61: goto st440;
		case 93: goto tr482;
	}
	goto tr134;
st440:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof440;
case 440:
	if ( (*( sm->p)) == 93 )
		goto tr134;
	goto tr491;
tr491:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st441;
st441:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof441;
case 441:
#line 6853 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 93 )
		goto tr493;
	goto st441;
st442:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof442;
case 442:
	switch( (*( sm->p)) ) {
		case 79: goto st443;
		case 111: goto st443;
	}
	goto tr134;
st443:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof443;
case 443:
	switch( (*( sm->p)) ) {
		case 73: goto st444;
		case 105: goto st444;
	}
	goto tr134;
st444:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof444;
case 444:
	switch( (*( sm->p)) ) {
		case 76: goto st445;
		case 108: goto st445;
	}
	goto tr134;
st445:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof445;
case 445:
	switch( (*( sm->p)) ) {
		case 69: goto st446;
		case 101: goto st446;
	}
	goto tr134;
st446:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof446;
case 446:
	switch( (*( sm->p)) ) {
		case 82: goto st447;
		case 114: goto st447;
	}
	goto tr134;
st447:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof447;
case 447:
	switch( (*( sm->p)) ) {
		case 83: goto st448;
		case 93: goto tr500;
		case 115: goto st448;
	}
	goto tr134;
st448:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof448;
case 448:
	if ( (*( sm->p)) == 93 )
		goto tr500;
	goto tr134;
st449:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof449;
case 449:
	switch( (*( sm->p)) ) {
		case 66: goto st450;
		case 80: goto st451;
		case 98: goto st450;
		case 112: goto st451;
	}
	goto tr134;
st450:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof450;
case 450:
	if ( (*( sm->p)) == 93 )
		goto tr503;
	goto tr134;
st451:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof451;
case 451:
	if ( (*( sm->p)) == 93 )
		goto tr504;
	goto tr134;
st452:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof452;
case 452:
	switch( (*( sm->p)) ) {
		case 65: goto st453;
		case 97: goto st453;
	}
	goto tr134;
st453:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof453;
case 453:
	switch( (*( sm->p)) ) {
		case 66: goto st454;
		case 98: goto st454;
	}
	goto tr134;
st454:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof454;
case 454:
	switch( (*( sm->p)) ) {
		case 76: goto st455;
		case 108: goto st455;
	}
	goto tr134;
st455:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof455;
case 455:
	switch( (*( sm->p)) ) {
		case 69: goto st456;
		case 101: goto st456;
	}
	goto tr134;
st456:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof456;
case 456:
	if ( (*( sm->p)) == 93 )
		goto tr509;
	goto tr134;
st457:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof457;
case 457:
	if ( (*( sm->p)) == 93 )
		goto tr510;
	goto tr134;
st458:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof458;
case 458:
	switch( (*( sm->p)) ) {
		case 79: goto st459;
		case 111: goto st459;
	}
	goto tr134;
st459:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof459;
case 459:
	switch( (*( sm->p)) ) {
		case 73: goto st460;
		case 105: goto st460;
	}
	goto tr134;
st460:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof460;
case 460:
	switch( (*( sm->p)) ) {
		case 68: goto st461;
		case 100: goto st461;
	}
	goto tr134;
st461:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof461;
case 461:
	if ( (*( sm->p)) == 93 )
		goto tr514;
	goto tr134;
st462:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof462;
case 462:
	switch( (*( sm->p)) ) {
		case 93: goto tr134;
		case 124: goto tr516;
	}
	goto tr515;
tr515:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st463;
st463:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof463;
case 463:
#line 7043 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 93: goto tr518;
		case 124: goto tr519;
	}
	goto st463;
tr518:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
	goto st464;
st464:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof464;
case 464:
#line 7055 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 93 )
		goto tr520;
	goto tr134;
tr519:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
	goto st465;
st465:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof465;
case 465:
#line 7065 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 93: goto tr134;
		case 124: goto tr134;
	}
	goto tr521;
tr521:
#line 94 "ext/dtext/dtext.cpp.rl"
	{ b1 = p; }
	goto st466;
st466:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof466;
case 466:
#line 7077 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 93: goto tr523;
		case 124: goto tr134;
	}
	goto st466;
tr523:
#line 95 "ext/dtext/dtext.cpp.rl"
	{ b2 = p; }
	goto st467;
st467:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof467;
case 467:
#line 7089 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 93 )
		goto tr524;
	goto tr134;
tr516:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st468;
st468:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof468;
case 468:
#line 7099 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 93: goto tr518;
		case 124: goto tr134;
	}
	goto st468;
st578:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof578;
case 578:
	if ( (*( sm->p)) == 96 )
		goto tr736;
	goto tr653;
tr638:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st579;
st579:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof579;
case 579:
#line 7118 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 123 )
		goto st469;
	goto tr653;
st469:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof469;
case 469:
	switch( (*( sm->p)) ) {
		case 124: goto tr527;
		case 125: goto tr134;
	}
	goto tr526;
tr526:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st470;
st470:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof470;
case 470:
#line 7137 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 124: goto tr529;
		case 125: goto tr530;
	}
	goto st470;
tr529:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
	goto st471;
st471:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof471;
case 471:
#line 7149 "ext/dtext/dtext.cpp"
	if ( 124 <= (*( sm->p)) && (*( sm->p)) <= 125 )
		goto tr134;
	goto tr531;
tr531:
#line 94 "ext/dtext/dtext.cpp.rl"
	{ b1 = p; }
	goto st472;
st472:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof472;
case 472:
#line 7159 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 124: goto tr134;
		case 125: goto tr533;
	}
	goto st472;
tr533:
#line 95 "ext/dtext/dtext.cpp.rl"
	{ b2 = p; }
	goto st473;
st473:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof473;
case 473:
#line 7171 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 125 )
		goto tr534;
	goto tr134;
tr530:
#line 93 "ext/dtext/dtext.cpp.rl"
	{ a2 = p; }
	goto st474;
st474:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof474;
case 474:
#line 7181 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 125 )
		goto tr535;
	goto tr134;
tr527:
#line 92 "ext/dtext/dtext.cpp.rl"
	{ a1 = p; }
	goto st475;
st475:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof475;
case 475:
#line 7191 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 124: goto tr134;
		case 125: goto tr530;
	}
	goto st475;
tr738:
#line 501 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    append_html_escaped((*( sm->p)));
  }}
	goto st580;
tr740:
#line 496 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    append("</span>");
    { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
  }}
	goto st580;
tr741:
#line 501 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_html_escaped((*( sm->p)));
  }}
	goto st580;
tr742:
#line 492 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    append("`");
  }}
	goto st580;
st580:
#line 1 "NONE"
	{( sm->ts) = 0;}
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof580;
case 580:
#line 1 "NONE"
	{( sm->ts) = ( sm->p);}
#line 7223 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 92: goto st581;
		case 96: goto tr740;
	}
	goto tr738;
st581:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof581;
case 581:
	if ( (*( sm->p)) == 96 )
		goto tr742;
	goto tr741;
tr537:
#line 516 "ext/dtext/dtext.cpp.rl"
	{{( sm->p) = ((( sm->te)))-1;}{
    append_html_escaped((*( sm->p)));
  }}
	goto st582;
tr542:
#line 507 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    if (dstack_check(BLOCK_CODE)) {
      dstack_rewind();
    } else {
      append("[/code]");
    }
    { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
  }}
	goto st582;
tr743:
#line 516 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    append_html_escaped((*( sm->p)));
  }}
	goto st582;
tr745:
#line 516 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;{
    append_html_escaped((*( sm->p)));
  }}
	goto st582;
st582:
#line 1 "NONE"
	{( sm->ts) = 0;}
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof582;
case 582:
#line 1 "NONE"
	{( sm->ts) = ( sm->p);}
#line 7266 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 91 )
		goto tr744;
	goto tr743;
tr744:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st583;
st583:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof583;
case 583:
#line 7276 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 47 )
		goto st476;
	goto tr745;
st476:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof476;
case 476:
	switch( (*( sm->p)) ) {
		case 67: goto st477;
		case 99: goto st477;
	}
	goto tr537;
st477:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof477;
case 477:
	switch( (*( sm->p)) ) {
		case 79: goto st478;
		case 111: goto st478;
	}
	goto tr537;
st478:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof478;
case 478:
	switch( (*( sm->p)) ) {
		case 68: goto st479;
		case 100: goto st479;
	}
	goto tr537;
st479:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof479;
case 479:
	switch( (*( sm->p)) ) {
		case 69: goto st480;
		case 101: goto st480;
	}
	goto tr537;
st480:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof480;
case 480:
	if ( (*( sm->p)) == 93 )
		goto tr542;
	goto tr537;
tr543:
#line 562 "ext/dtext/dtext.cpp.rl"
	{{( sm->p) = ((( sm->te)))-1;}}
	goto st584;
tr552:
#line 556 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    if (dstack_close_block(BLOCK_TABLE, "</table>")) {
      { sm->cs = ( (sm->stack.data()))[--( sm->top)];goto _again;}
    }
  }}
	goto st584;
tr556:
#line 534 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_close_block(BLOCK_TBODY, "</tbody>");
  }}
	goto st584;
tr560:
#line 526 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_close_block(BLOCK_THEAD, "</thead>");
  }}
	goto st584;
tr561:
#line 547 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_close_block(BLOCK_TR, "</tr>");
  }}
	goto st584;
tr569:
#line 530 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_open_block(BLOCK_TBODY, "<tbody>");
  }}
	goto st584;
tr570:
#line 551 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_open_block(BLOCK_TD, "<td>");
    {
  size_t len = stack.size();

  // Should never happen.
  if (len > MAX_STACK_DEPTH) {
    throw DTextError("too many nested elements");
  }

  if (top >= len) {
    g_debug("growing stack %zi\n", len + 16);
    stack.resize(len + 16, 0);
  }
{( (sm->stack.data()))[( sm->top)++] = 584;goto st526;}}
  }}
	goto st584;
tr572:
#line 538 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_open_block(BLOCK_TH, "<th>");
    {
  size_t len = stack.size();

  // Should never happen.
  if (len > MAX_STACK_DEPTH) {
    throw DTextError("too many nested elements");
  }

  if (top >= len) {
    g_debug("growing stack %zi\n", len + 16);
    stack.resize(len + 16, 0);
  }
{( (sm->stack.data()))[( sm->top)++] = 584;goto st526;}}
  }}
	goto st584;
tr575:
#line 522 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_open_block(BLOCK_THEAD, "<thead>");
  }}
	goto st584;
tr576:
#line 543 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;{
    dstack_open_block(BLOCK_TR, "<tr>");
  }}
	goto st584;
tr747:
#line 562 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p)+1;}
	goto st584;
tr749:
#line 562 "ext/dtext/dtext.cpp.rl"
	{( sm->te) = ( sm->p);( sm->p)--;}
	goto st584;
st584:
#line 1 "NONE"
	{( sm->ts) = 0;}
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof584;
case 584:
#line 1 "NONE"
	{( sm->ts) = ( sm->p);}
#line 7410 "ext/dtext/dtext.cpp"
	if ( (*( sm->p)) == 91 )
		goto tr748;
	goto tr747;
tr748:
#line 1 "NONE"
	{( sm->te) = ( sm->p)+1;}
	goto st585;
st585:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof585;
case 585:
#line 7420 "ext/dtext/dtext.cpp"
	switch( (*( sm->p)) ) {
		case 47: goto st481;
		case 84: goto st496;
		case 116: goto st496;
	}
	goto tr749;
st481:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof481;
case 481:
	switch( (*( sm->p)) ) {
		case 84: goto st482;
		case 116: goto st482;
	}
	goto tr543;
st482:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof482;
case 482:
	switch( (*( sm->p)) ) {
		case 65: goto st483;
		case 66: goto st487;
		case 72: goto st491;
		case 82: goto st495;
		case 97: goto st483;
		case 98: goto st487;
		case 104: goto st491;
		case 114: goto st495;
	}
	goto tr543;
st483:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof483;
case 483:
	switch( (*( sm->p)) ) {
		case 66: goto st484;
		case 98: goto st484;
	}
	goto tr543;
st484:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof484;
case 484:
	switch( (*( sm->p)) ) {
		case 76: goto st485;
		case 108: goto st485;
	}
	goto tr543;
st485:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof485;
case 485:
	switch( (*( sm->p)) ) {
		case 69: goto st486;
		case 101: goto st486;
	}
	goto tr543;
st486:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof486;
case 486:
	if ( (*( sm->p)) == 93 )
		goto tr552;
	goto tr543;
st487:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof487;
case 487:
	switch( (*( sm->p)) ) {
		case 79: goto st488;
		case 111: goto st488;
	}
	goto tr543;
st488:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof488;
case 488:
	switch( (*( sm->p)) ) {
		case 68: goto st489;
		case 100: goto st489;
	}
	goto tr543;
st489:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof489;
case 489:
	switch( (*( sm->p)) ) {
		case 89: goto st490;
		case 121: goto st490;
	}
	goto tr543;
st490:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof490;
case 490:
	if ( (*( sm->p)) == 93 )
		goto tr556;
	goto tr543;
st491:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof491;
case 491:
	switch( (*( sm->p)) ) {
		case 69: goto st492;
		case 101: goto st492;
	}
	goto tr543;
st492:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof492;
case 492:
	switch( (*( sm->p)) ) {
		case 65: goto st493;
		case 97: goto st493;
	}
	goto tr543;
st493:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof493;
case 493:
	switch( (*( sm->p)) ) {
		case 68: goto st494;
		case 100: goto st494;
	}
	goto tr543;
st494:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof494;
case 494:
	if ( (*( sm->p)) == 93 )
		goto tr560;
	goto tr543;
st495:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof495;
case 495:
	if ( (*( sm->p)) == 93 )
		goto tr561;
	goto tr543;
st496:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof496;
case 496:
	switch( (*( sm->p)) ) {
		case 66: goto st497;
		case 68: goto st501;
		case 72: goto st502;
		case 82: goto st506;
		case 98: goto st497;
		case 100: goto st501;
		case 104: goto st502;
		case 114: goto st506;
	}
	goto tr543;
st497:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof497;
case 497:
	switch( (*( sm->p)) ) {
		case 79: goto st498;
		case 111: goto st498;
	}
	goto tr543;
st498:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof498;
case 498:
	switch( (*( sm->p)) ) {
		case 68: goto st499;
		case 100: goto st499;
	}
	goto tr543;
st499:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof499;
case 499:
	switch( (*( sm->p)) ) {
		case 89: goto st500;
		case 121: goto st500;
	}
	goto tr543;
st500:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof500;
case 500:
	if ( (*( sm->p)) == 93 )
		goto tr569;
	goto tr543;
st501:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof501;
case 501:
	if ( (*( sm->p)) == 93 )
		goto tr570;
	goto tr543;
st502:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof502;
case 502:
	switch( (*( sm->p)) ) {
		case 69: goto st503;
		case 93: goto tr572;
		case 101: goto st503;
	}
	goto tr543;
st503:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof503;
case 503:
	switch( (*( sm->p)) ) {
		case 65: goto st504;
		case 97: goto st504;
	}
	goto tr543;
st504:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof504;
case 504:
	switch( (*( sm->p)) ) {
		case 68: goto st505;
		case 100: goto st505;
	}
	goto tr543;
st505:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof505;
case 505:
	if ( (*( sm->p)) == 93 )
		goto tr575;
	goto tr543;
st506:
	if ( ++( sm->p) == ( sm->pe) )
		goto _test_eof506;
case 506:
	if ( (*( sm->p)) == 93 )
		goto tr576;
	goto tr543;
	}
	_test_eof507:  sm->cs = 507; goto _test_eof; 
	_test_eof508:  sm->cs = 508; goto _test_eof; 
	_test_eof0:  sm->cs = 0; goto _test_eof; 
	_test_eof509:  sm->cs = 509; goto _test_eof; 
	_test_eof510:  sm->cs = 510; goto _test_eof; 
	_test_eof1:  sm->cs = 1; goto _test_eof; 
	_test_eof511:  sm->cs = 511; goto _test_eof; 
	_test_eof512:  sm->cs = 512; goto _test_eof; 
	_test_eof2:  sm->cs = 2; goto _test_eof; 
	_test_eof513:  sm->cs = 513; goto _test_eof; 
	_test_eof3:  sm->cs = 3; goto _test_eof; 
	_test_eof514:  sm->cs = 514; goto _test_eof; 
	_test_eof515:  sm->cs = 515; goto _test_eof; 
	_test_eof4:  sm->cs = 4; goto _test_eof; 
	_test_eof5:  sm->cs = 5; goto _test_eof; 
	_test_eof6:  sm->cs = 6; goto _test_eof; 
	_test_eof7:  sm->cs = 7; goto _test_eof; 
	_test_eof8:  sm->cs = 8; goto _test_eof; 
	_test_eof9:  sm->cs = 9; goto _test_eof; 
	_test_eof10:  sm->cs = 10; goto _test_eof; 
	_test_eof11:  sm->cs = 11; goto _test_eof; 
	_test_eof12:  sm->cs = 12; goto _test_eof; 
	_test_eof13:  sm->cs = 13; goto _test_eof; 
	_test_eof14:  sm->cs = 14; goto _test_eof; 
	_test_eof15:  sm->cs = 15; goto _test_eof; 
	_test_eof16:  sm->cs = 16; goto _test_eof; 
	_test_eof516:  sm->cs = 516; goto _test_eof; 
	_test_eof17:  sm->cs = 17; goto _test_eof; 
	_test_eof18:  sm->cs = 18; goto _test_eof; 
	_test_eof19:  sm->cs = 19; goto _test_eof; 
	_test_eof20:  sm->cs = 20; goto _test_eof; 
	_test_eof21:  sm->cs = 21; goto _test_eof; 
	_test_eof517:  sm->cs = 517; goto _test_eof; 
	_test_eof22:  sm->cs = 22; goto _test_eof; 
	_test_eof23:  sm->cs = 23; goto _test_eof; 
	_test_eof24:  sm->cs = 24; goto _test_eof; 
	_test_eof25:  sm->cs = 25; goto _test_eof; 
	_test_eof26:  sm->cs = 26; goto _test_eof; 
	_test_eof27:  sm->cs = 27; goto _test_eof; 
	_test_eof28:  sm->cs = 28; goto _test_eof; 
	_test_eof29:  sm->cs = 29; goto _test_eof; 
	_test_eof30:  sm->cs = 30; goto _test_eof; 
	_test_eof31:  sm->cs = 31; goto _test_eof; 
	_test_eof32:  sm->cs = 32; goto _test_eof; 
	_test_eof33:  sm->cs = 33; goto _test_eof; 
	_test_eof34:  sm->cs = 34; goto _test_eof; 
	_test_eof35:  sm->cs = 35; goto _test_eof; 
	_test_eof36:  sm->cs = 36; goto _test_eof; 
	_test_eof37:  sm->cs = 37; goto _test_eof; 
	_test_eof38:  sm->cs = 38; goto _test_eof; 
	_test_eof39:  sm->cs = 39; goto _test_eof; 
	_test_eof518:  sm->cs = 518; goto _test_eof; 
	_test_eof519:  sm->cs = 519; goto _test_eof; 
	_test_eof40:  sm->cs = 40; goto _test_eof; 
	_test_eof41:  sm->cs = 41; goto _test_eof; 
	_test_eof520:  sm->cs = 520; goto _test_eof; 
	_test_eof521:  sm->cs = 521; goto _test_eof; 
	_test_eof42:  sm->cs = 42; goto _test_eof; 
	_test_eof43:  sm->cs = 43; goto _test_eof; 
	_test_eof44:  sm->cs = 44; goto _test_eof; 
	_test_eof45:  sm->cs = 45; goto _test_eof; 
	_test_eof46:  sm->cs = 46; goto _test_eof; 
	_test_eof47:  sm->cs = 47; goto _test_eof; 
	_test_eof48:  sm->cs = 48; goto _test_eof; 
	_test_eof522:  sm->cs = 522; goto _test_eof; 
	_test_eof49:  sm->cs = 49; goto _test_eof; 
	_test_eof50:  sm->cs = 50; goto _test_eof; 
	_test_eof51:  sm->cs = 51; goto _test_eof; 
	_test_eof52:  sm->cs = 52; goto _test_eof; 
	_test_eof53:  sm->cs = 53; goto _test_eof; 
	_test_eof523:  sm->cs = 523; goto _test_eof; 
	_test_eof524:  sm->cs = 524; goto _test_eof; 
	_test_eof525:  sm->cs = 525; goto _test_eof; 
	_test_eof54:  sm->cs = 54; goto _test_eof; 
	_test_eof55:  sm->cs = 55; goto _test_eof; 
	_test_eof56:  sm->cs = 56; goto _test_eof; 
	_test_eof57:  sm->cs = 57; goto _test_eof; 
	_test_eof58:  sm->cs = 58; goto _test_eof; 
	_test_eof59:  sm->cs = 59; goto _test_eof; 
	_test_eof60:  sm->cs = 60; goto _test_eof; 
	_test_eof61:  sm->cs = 61; goto _test_eof; 
	_test_eof62:  sm->cs = 62; goto _test_eof; 
	_test_eof63:  sm->cs = 63; goto _test_eof; 
	_test_eof64:  sm->cs = 64; goto _test_eof; 
	_test_eof65:  sm->cs = 65; goto _test_eof; 
	_test_eof66:  sm->cs = 66; goto _test_eof; 
	_test_eof67:  sm->cs = 67; goto _test_eof; 
	_test_eof68:  sm->cs = 68; goto _test_eof; 
	_test_eof69:  sm->cs = 69; goto _test_eof; 
	_test_eof70:  sm->cs = 70; goto _test_eof; 
	_test_eof71:  sm->cs = 71; goto _test_eof; 
	_test_eof72:  sm->cs = 72; goto _test_eof; 
	_test_eof73:  sm->cs = 73; goto _test_eof; 
	_test_eof74:  sm->cs = 74; goto _test_eof; 
	_test_eof75:  sm->cs = 75; goto _test_eof; 
	_test_eof76:  sm->cs = 76; goto _test_eof; 
	_test_eof526:  sm->cs = 526; goto _test_eof; 
	_test_eof527:  sm->cs = 527; goto _test_eof; 
	_test_eof528:  sm->cs = 528; goto _test_eof; 
	_test_eof77:  sm->cs = 77; goto _test_eof; 
	_test_eof78:  sm->cs = 78; goto _test_eof; 
	_test_eof79:  sm->cs = 79; goto _test_eof; 
	_test_eof80:  sm->cs = 80; goto _test_eof; 
	_test_eof81:  sm->cs = 81; goto _test_eof; 
	_test_eof82:  sm->cs = 82; goto _test_eof; 
	_test_eof83:  sm->cs = 83; goto _test_eof; 
	_test_eof84:  sm->cs = 84; goto _test_eof; 
	_test_eof85:  sm->cs = 85; goto _test_eof; 
	_test_eof86:  sm->cs = 86; goto _test_eof; 
	_test_eof87:  sm->cs = 87; goto _test_eof; 
	_test_eof88:  sm->cs = 88; goto _test_eof; 
	_test_eof89:  sm->cs = 89; goto _test_eof; 
	_test_eof90:  sm->cs = 90; goto _test_eof; 
	_test_eof91:  sm->cs = 91; goto _test_eof; 
	_test_eof529:  sm->cs = 529; goto _test_eof; 
	_test_eof530:  sm->cs = 530; goto _test_eof; 
	_test_eof92:  sm->cs = 92; goto _test_eof; 
	_test_eof93:  sm->cs = 93; goto _test_eof; 
	_test_eof531:  sm->cs = 531; goto _test_eof; 
	_test_eof94:  sm->cs = 94; goto _test_eof; 
	_test_eof95:  sm->cs = 95; goto _test_eof; 
	_test_eof96:  sm->cs = 96; goto _test_eof; 
	_test_eof97:  sm->cs = 97; goto _test_eof; 
	_test_eof98:  sm->cs = 98; goto _test_eof; 
	_test_eof99:  sm->cs = 99; goto _test_eof; 
	_test_eof100:  sm->cs = 100; goto _test_eof; 
	_test_eof532:  sm->cs = 532; goto _test_eof; 
	_test_eof101:  sm->cs = 101; goto _test_eof; 
	_test_eof102:  sm->cs = 102; goto _test_eof; 
	_test_eof103:  sm->cs = 103; goto _test_eof; 
	_test_eof104:  sm->cs = 104; goto _test_eof; 
	_test_eof105:  sm->cs = 105; goto _test_eof; 
	_test_eof106:  sm->cs = 106; goto _test_eof; 
	_test_eof107:  sm->cs = 107; goto _test_eof; 
	_test_eof533:  sm->cs = 533; goto _test_eof; 
	_test_eof534:  sm->cs = 534; goto _test_eof; 
	_test_eof535:  sm->cs = 535; goto _test_eof; 
	_test_eof108:  sm->cs = 108; goto _test_eof; 
	_test_eof109:  sm->cs = 109; goto _test_eof; 
	_test_eof110:  sm->cs = 110; goto _test_eof; 
	_test_eof111:  sm->cs = 111; goto _test_eof; 
	_test_eof536:  sm->cs = 536; goto _test_eof; 
	_test_eof112:  sm->cs = 112; goto _test_eof; 
	_test_eof113:  sm->cs = 113; goto _test_eof; 
	_test_eof114:  sm->cs = 114; goto _test_eof; 
	_test_eof115:  sm->cs = 115; goto _test_eof; 
	_test_eof116:  sm->cs = 116; goto _test_eof; 
	_test_eof117:  sm->cs = 117; goto _test_eof; 
	_test_eof118:  sm->cs = 118; goto _test_eof; 
	_test_eof119:  sm->cs = 119; goto _test_eof; 
	_test_eof120:  sm->cs = 120; goto _test_eof; 
	_test_eof121:  sm->cs = 121; goto _test_eof; 
	_test_eof122:  sm->cs = 122; goto _test_eof; 
	_test_eof123:  sm->cs = 123; goto _test_eof; 
	_test_eof124:  sm->cs = 124; goto _test_eof; 
	_test_eof125:  sm->cs = 125; goto _test_eof; 
	_test_eof126:  sm->cs = 126; goto _test_eof; 
	_test_eof127:  sm->cs = 127; goto _test_eof; 
	_test_eof128:  sm->cs = 128; goto _test_eof; 
	_test_eof537:  sm->cs = 537; goto _test_eof; 
	_test_eof129:  sm->cs = 129; goto _test_eof; 
	_test_eof130:  sm->cs = 130; goto _test_eof; 
	_test_eof131:  sm->cs = 131; goto _test_eof; 
	_test_eof132:  sm->cs = 132; goto _test_eof; 
	_test_eof133:  sm->cs = 133; goto _test_eof; 
	_test_eof134:  sm->cs = 134; goto _test_eof; 
	_test_eof135:  sm->cs = 135; goto _test_eof; 
	_test_eof136:  sm->cs = 136; goto _test_eof; 
	_test_eof137:  sm->cs = 137; goto _test_eof; 
	_test_eof538:  sm->cs = 538; goto _test_eof; 
	_test_eof138:  sm->cs = 138; goto _test_eof; 
	_test_eof139:  sm->cs = 139; goto _test_eof; 
	_test_eof140:  sm->cs = 140; goto _test_eof; 
	_test_eof141:  sm->cs = 141; goto _test_eof; 
	_test_eof142:  sm->cs = 142; goto _test_eof; 
	_test_eof143:  sm->cs = 143; goto _test_eof; 
	_test_eof539:  sm->cs = 539; goto _test_eof; 
	_test_eof144:  sm->cs = 144; goto _test_eof; 
	_test_eof145:  sm->cs = 145; goto _test_eof; 
	_test_eof146:  sm->cs = 146; goto _test_eof; 
	_test_eof147:  sm->cs = 147; goto _test_eof; 
	_test_eof148:  sm->cs = 148; goto _test_eof; 
	_test_eof149:  sm->cs = 149; goto _test_eof; 
	_test_eof150:  sm->cs = 150; goto _test_eof; 
	_test_eof540:  sm->cs = 540; goto _test_eof; 
	_test_eof541:  sm->cs = 541; goto _test_eof; 
	_test_eof151:  sm->cs = 151; goto _test_eof; 
	_test_eof152:  sm->cs = 152; goto _test_eof; 
	_test_eof153:  sm->cs = 153; goto _test_eof; 
	_test_eof154:  sm->cs = 154; goto _test_eof; 
	_test_eof542:  sm->cs = 542; goto _test_eof; 
	_test_eof155:  sm->cs = 155; goto _test_eof; 
	_test_eof156:  sm->cs = 156; goto _test_eof; 
	_test_eof157:  sm->cs = 157; goto _test_eof; 
	_test_eof158:  sm->cs = 158; goto _test_eof; 
	_test_eof159:  sm->cs = 159; goto _test_eof; 
	_test_eof543:  sm->cs = 543; goto _test_eof; 
	_test_eof160:  sm->cs = 160; goto _test_eof; 
	_test_eof161:  sm->cs = 161; goto _test_eof; 
	_test_eof162:  sm->cs = 162; goto _test_eof; 
	_test_eof163:  sm->cs = 163; goto _test_eof; 
	_test_eof544:  sm->cs = 544; goto _test_eof; 
	_test_eof545:  sm->cs = 545; goto _test_eof; 
	_test_eof164:  sm->cs = 164; goto _test_eof; 
	_test_eof165:  sm->cs = 165; goto _test_eof; 
	_test_eof166:  sm->cs = 166; goto _test_eof; 
	_test_eof167:  sm->cs = 167; goto _test_eof; 
	_test_eof168:  sm->cs = 168; goto _test_eof; 
	_test_eof169:  sm->cs = 169; goto _test_eof; 
	_test_eof170:  sm->cs = 170; goto _test_eof; 
	_test_eof171:  sm->cs = 171; goto _test_eof; 
	_test_eof546:  sm->cs = 546; goto _test_eof; 
	_test_eof547:  sm->cs = 547; goto _test_eof; 
	_test_eof172:  sm->cs = 172; goto _test_eof; 
	_test_eof173:  sm->cs = 173; goto _test_eof; 
	_test_eof174:  sm->cs = 174; goto _test_eof; 
	_test_eof175:  sm->cs = 175; goto _test_eof; 
	_test_eof176:  sm->cs = 176; goto _test_eof; 
	_test_eof548:  sm->cs = 548; goto _test_eof; 
	_test_eof177:  sm->cs = 177; goto _test_eof; 
	_test_eof178:  sm->cs = 178; goto _test_eof; 
	_test_eof179:  sm->cs = 179; goto _test_eof; 
	_test_eof180:  sm->cs = 180; goto _test_eof; 
	_test_eof181:  sm->cs = 181; goto _test_eof; 
	_test_eof182:  sm->cs = 182; goto _test_eof; 
	_test_eof549:  sm->cs = 549; goto _test_eof; 
	_test_eof550:  sm->cs = 550; goto _test_eof; 
	_test_eof183:  sm->cs = 183; goto _test_eof; 
	_test_eof184:  sm->cs = 184; goto _test_eof; 
	_test_eof185:  sm->cs = 185; goto _test_eof; 
	_test_eof186:  sm->cs = 186; goto _test_eof; 
	_test_eof187:  sm->cs = 187; goto _test_eof; 
	_test_eof188:  sm->cs = 188; goto _test_eof; 
	_test_eof551:  sm->cs = 551; goto _test_eof; 
	_test_eof189:  sm->cs = 189; goto _test_eof; 
	_test_eof552:  sm->cs = 552; goto _test_eof; 
	_test_eof190:  sm->cs = 190; goto _test_eof; 
	_test_eof191:  sm->cs = 191; goto _test_eof; 
	_test_eof192:  sm->cs = 192; goto _test_eof; 
	_test_eof193:  sm->cs = 193; goto _test_eof; 
	_test_eof194:  sm->cs = 194; goto _test_eof; 
	_test_eof195:  sm->cs = 195; goto _test_eof; 
	_test_eof196:  sm->cs = 196; goto _test_eof; 
	_test_eof197:  sm->cs = 197; goto _test_eof; 
	_test_eof198:  sm->cs = 198; goto _test_eof; 
	_test_eof199:  sm->cs = 199; goto _test_eof; 
	_test_eof200:  sm->cs = 200; goto _test_eof; 
	_test_eof201:  sm->cs = 201; goto _test_eof; 
	_test_eof553:  sm->cs = 553; goto _test_eof; 
	_test_eof554:  sm->cs = 554; goto _test_eof; 
	_test_eof202:  sm->cs = 202; goto _test_eof; 
	_test_eof203:  sm->cs = 203; goto _test_eof; 
	_test_eof204:  sm->cs = 204; goto _test_eof; 
	_test_eof205:  sm->cs = 205; goto _test_eof; 
	_test_eof206:  sm->cs = 206; goto _test_eof; 
	_test_eof207:  sm->cs = 207; goto _test_eof; 
	_test_eof208:  sm->cs = 208; goto _test_eof; 
	_test_eof209:  sm->cs = 209; goto _test_eof; 
	_test_eof210:  sm->cs = 210; goto _test_eof; 
	_test_eof211:  sm->cs = 211; goto _test_eof; 
	_test_eof212:  sm->cs = 212; goto _test_eof; 
	_test_eof555:  sm->cs = 555; goto _test_eof; 
	_test_eof556:  sm->cs = 556; goto _test_eof; 
	_test_eof213:  sm->cs = 213; goto _test_eof; 
	_test_eof214:  sm->cs = 214; goto _test_eof; 
	_test_eof215:  sm->cs = 215; goto _test_eof; 
	_test_eof216:  sm->cs = 216; goto _test_eof; 
	_test_eof217:  sm->cs = 217; goto _test_eof; 
	_test_eof557:  sm->cs = 557; goto _test_eof; 
	_test_eof558:  sm->cs = 558; goto _test_eof; 
	_test_eof218:  sm->cs = 218; goto _test_eof; 
	_test_eof219:  sm->cs = 219; goto _test_eof; 
	_test_eof220:  sm->cs = 220; goto _test_eof; 
	_test_eof221:  sm->cs = 221; goto _test_eof; 
	_test_eof222:  sm->cs = 222; goto _test_eof; 
	_test_eof559:  sm->cs = 559; goto _test_eof; 
	_test_eof223:  sm->cs = 223; goto _test_eof; 
	_test_eof224:  sm->cs = 224; goto _test_eof; 
	_test_eof225:  sm->cs = 225; goto _test_eof; 
	_test_eof226:  sm->cs = 226; goto _test_eof; 
	_test_eof560:  sm->cs = 560; goto _test_eof; 
	_test_eof227:  sm->cs = 227; goto _test_eof; 
	_test_eof228:  sm->cs = 228; goto _test_eof; 
	_test_eof229:  sm->cs = 229; goto _test_eof; 
	_test_eof230:  sm->cs = 230; goto _test_eof; 
	_test_eof231:  sm->cs = 231; goto _test_eof; 
	_test_eof232:  sm->cs = 232; goto _test_eof; 
	_test_eof233:  sm->cs = 233; goto _test_eof; 
	_test_eof234:  sm->cs = 234; goto _test_eof; 
	_test_eof235:  sm->cs = 235; goto _test_eof; 
	_test_eof561:  sm->cs = 561; goto _test_eof; 
	_test_eof562:  sm->cs = 562; goto _test_eof; 
	_test_eof236:  sm->cs = 236; goto _test_eof; 
	_test_eof237:  sm->cs = 237; goto _test_eof; 
	_test_eof238:  sm->cs = 238; goto _test_eof; 
	_test_eof239:  sm->cs = 239; goto _test_eof; 
	_test_eof240:  sm->cs = 240; goto _test_eof; 
	_test_eof241:  sm->cs = 241; goto _test_eof; 
	_test_eof242:  sm->cs = 242; goto _test_eof; 
	_test_eof563:  sm->cs = 563; goto _test_eof; 
	_test_eof564:  sm->cs = 564; goto _test_eof; 
	_test_eof243:  sm->cs = 243; goto _test_eof; 
	_test_eof244:  sm->cs = 244; goto _test_eof; 
	_test_eof245:  sm->cs = 245; goto _test_eof; 
	_test_eof246:  sm->cs = 246; goto _test_eof; 
	_test_eof565:  sm->cs = 565; goto _test_eof; 
	_test_eof566:  sm->cs = 566; goto _test_eof; 
	_test_eof247:  sm->cs = 247; goto _test_eof; 
	_test_eof248:  sm->cs = 248; goto _test_eof; 
	_test_eof249:  sm->cs = 249; goto _test_eof; 
	_test_eof250:  sm->cs = 250; goto _test_eof; 
	_test_eof251:  sm->cs = 251; goto _test_eof; 
	_test_eof252:  sm->cs = 252; goto _test_eof; 
	_test_eof253:  sm->cs = 253; goto _test_eof; 
	_test_eof254:  sm->cs = 254; goto _test_eof; 
	_test_eof255:  sm->cs = 255; goto _test_eof; 
	_test_eof256:  sm->cs = 256; goto _test_eof; 
	_test_eof567:  sm->cs = 567; goto _test_eof; 
	_test_eof257:  sm->cs = 257; goto _test_eof; 
	_test_eof258:  sm->cs = 258; goto _test_eof; 
	_test_eof259:  sm->cs = 259; goto _test_eof; 
	_test_eof260:  sm->cs = 260; goto _test_eof; 
	_test_eof261:  sm->cs = 261; goto _test_eof; 
	_test_eof262:  sm->cs = 262; goto _test_eof; 
	_test_eof263:  sm->cs = 263; goto _test_eof; 
	_test_eof264:  sm->cs = 264; goto _test_eof; 
	_test_eof265:  sm->cs = 265; goto _test_eof; 
	_test_eof266:  sm->cs = 266; goto _test_eof; 
	_test_eof267:  sm->cs = 267; goto _test_eof; 
	_test_eof268:  sm->cs = 268; goto _test_eof; 
	_test_eof269:  sm->cs = 269; goto _test_eof; 
	_test_eof270:  sm->cs = 270; goto _test_eof; 
	_test_eof568:  sm->cs = 568; goto _test_eof; 
	_test_eof271:  sm->cs = 271; goto _test_eof; 
	_test_eof272:  sm->cs = 272; goto _test_eof; 
	_test_eof273:  sm->cs = 273; goto _test_eof; 
	_test_eof274:  sm->cs = 274; goto _test_eof; 
	_test_eof275:  sm->cs = 275; goto _test_eof; 
	_test_eof276:  sm->cs = 276; goto _test_eof; 
	_test_eof277:  sm->cs = 277; goto _test_eof; 
	_test_eof569:  sm->cs = 569; goto _test_eof; 
	_test_eof278:  sm->cs = 278; goto _test_eof; 
	_test_eof279:  sm->cs = 279; goto _test_eof; 
	_test_eof280:  sm->cs = 280; goto _test_eof; 
	_test_eof281:  sm->cs = 281; goto _test_eof; 
	_test_eof282:  sm->cs = 282; goto _test_eof; 
	_test_eof283:  sm->cs = 283; goto _test_eof; 
	_test_eof570:  sm->cs = 570; goto _test_eof; 
	_test_eof571:  sm->cs = 571; goto _test_eof; 
	_test_eof284:  sm->cs = 284; goto _test_eof; 
	_test_eof285:  sm->cs = 285; goto _test_eof; 
	_test_eof286:  sm->cs = 286; goto _test_eof; 
	_test_eof287:  sm->cs = 287; goto _test_eof; 
	_test_eof288:  sm->cs = 288; goto _test_eof; 
	_test_eof572:  sm->cs = 572; goto _test_eof; 
	_test_eof573:  sm->cs = 573; goto _test_eof; 
	_test_eof289:  sm->cs = 289; goto _test_eof; 
	_test_eof290:  sm->cs = 290; goto _test_eof; 
	_test_eof291:  sm->cs = 291; goto _test_eof; 
	_test_eof292:  sm->cs = 292; goto _test_eof; 
	_test_eof293:  sm->cs = 293; goto _test_eof; 
	_test_eof574:  sm->cs = 574; goto _test_eof; 
	_test_eof575:  sm->cs = 575; goto _test_eof; 
	_test_eof294:  sm->cs = 294; goto _test_eof; 
	_test_eof295:  sm->cs = 295; goto _test_eof; 
	_test_eof296:  sm->cs = 296; goto _test_eof; 
	_test_eof297:  sm->cs = 297; goto _test_eof; 
	_test_eof298:  sm->cs = 298; goto _test_eof; 
	_test_eof299:  sm->cs = 299; goto _test_eof; 
	_test_eof300:  sm->cs = 300; goto _test_eof; 
	_test_eof301:  sm->cs = 301; goto _test_eof; 
	_test_eof576:  sm->cs = 576; goto _test_eof; 
	_test_eof302:  sm->cs = 302; goto _test_eof; 
	_test_eof303:  sm->cs = 303; goto _test_eof; 
	_test_eof304:  sm->cs = 304; goto _test_eof; 
	_test_eof305:  sm->cs = 305; goto _test_eof; 
	_test_eof306:  sm->cs = 306; goto _test_eof; 
	_test_eof307:  sm->cs = 307; goto _test_eof; 
	_test_eof308:  sm->cs = 308; goto _test_eof; 
	_test_eof309:  sm->cs = 309; goto _test_eof; 
	_test_eof310:  sm->cs = 310; goto _test_eof; 
	_test_eof311:  sm->cs = 311; goto _test_eof; 
	_test_eof312:  sm->cs = 312; goto _test_eof; 
	_test_eof313:  sm->cs = 313; goto _test_eof; 
	_test_eof314:  sm->cs = 314; goto _test_eof; 
	_test_eof577:  sm->cs = 577; goto _test_eof; 
	_test_eof315:  sm->cs = 315; goto _test_eof; 
	_test_eof316:  sm->cs = 316; goto _test_eof; 
	_test_eof317:  sm->cs = 317; goto _test_eof; 
	_test_eof318:  sm->cs = 318; goto _test_eof; 
	_test_eof319:  sm->cs = 319; goto _test_eof; 
	_test_eof320:  sm->cs = 320; goto _test_eof; 
	_test_eof321:  sm->cs = 321; goto _test_eof; 
	_test_eof322:  sm->cs = 322; goto _test_eof; 
	_test_eof323:  sm->cs = 323; goto _test_eof; 
	_test_eof324:  sm->cs = 324; goto _test_eof; 
	_test_eof325:  sm->cs = 325; goto _test_eof; 
	_test_eof326:  sm->cs = 326; goto _test_eof; 
	_test_eof327:  sm->cs = 327; goto _test_eof; 
	_test_eof328:  sm->cs = 328; goto _test_eof; 
	_test_eof329:  sm->cs = 329; goto _test_eof; 
	_test_eof330:  sm->cs = 330; goto _test_eof; 
	_test_eof331:  sm->cs = 331; goto _test_eof; 
	_test_eof332:  sm->cs = 332; goto _test_eof; 
	_test_eof333:  sm->cs = 333; goto _test_eof; 
	_test_eof334:  sm->cs = 334; goto _test_eof; 
	_test_eof335:  sm->cs = 335; goto _test_eof; 
	_test_eof336:  sm->cs = 336; goto _test_eof; 
	_test_eof337:  sm->cs = 337; goto _test_eof; 
	_test_eof338:  sm->cs = 338; goto _test_eof; 
	_test_eof339:  sm->cs = 339; goto _test_eof; 
	_test_eof340:  sm->cs = 340; goto _test_eof; 
	_test_eof341:  sm->cs = 341; goto _test_eof; 
	_test_eof342:  sm->cs = 342; goto _test_eof; 
	_test_eof343:  sm->cs = 343; goto _test_eof; 
	_test_eof344:  sm->cs = 344; goto _test_eof; 
	_test_eof345:  sm->cs = 345; goto _test_eof; 
	_test_eof346:  sm->cs = 346; goto _test_eof; 
	_test_eof347:  sm->cs = 347; goto _test_eof; 
	_test_eof348:  sm->cs = 348; goto _test_eof; 
	_test_eof349:  sm->cs = 349; goto _test_eof; 
	_test_eof350:  sm->cs = 350; goto _test_eof; 
	_test_eof351:  sm->cs = 351; goto _test_eof; 
	_test_eof352:  sm->cs = 352; goto _test_eof; 
	_test_eof353:  sm->cs = 353; goto _test_eof; 
	_test_eof354:  sm->cs = 354; goto _test_eof; 
	_test_eof355:  sm->cs = 355; goto _test_eof; 
	_test_eof356:  sm->cs = 356; goto _test_eof; 
	_test_eof357:  sm->cs = 357; goto _test_eof; 
	_test_eof358:  sm->cs = 358; goto _test_eof; 
	_test_eof359:  sm->cs = 359; goto _test_eof; 
	_test_eof360:  sm->cs = 360; goto _test_eof; 
	_test_eof361:  sm->cs = 361; goto _test_eof; 
	_test_eof362:  sm->cs = 362; goto _test_eof; 
	_test_eof363:  sm->cs = 363; goto _test_eof; 
	_test_eof364:  sm->cs = 364; goto _test_eof; 
	_test_eof365:  sm->cs = 365; goto _test_eof; 
	_test_eof366:  sm->cs = 366; goto _test_eof; 
	_test_eof367:  sm->cs = 367; goto _test_eof; 
	_test_eof368:  sm->cs = 368; goto _test_eof; 
	_test_eof369:  sm->cs = 369; goto _test_eof; 
	_test_eof370:  sm->cs = 370; goto _test_eof; 
	_test_eof371:  sm->cs = 371; goto _test_eof; 
	_test_eof372:  sm->cs = 372; goto _test_eof; 
	_test_eof373:  sm->cs = 373; goto _test_eof; 
	_test_eof374:  sm->cs = 374; goto _test_eof; 
	_test_eof375:  sm->cs = 375; goto _test_eof; 
	_test_eof376:  sm->cs = 376; goto _test_eof; 
	_test_eof377:  sm->cs = 377; goto _test_eof; 
	_test_eof378:  sm->cs = 378; goto _test_eof; 
	_test_eof379:  sm->cs = 379; goto _test_eof; 
	_test_eof380:  sm->cs = 380; goto _test_eof; 
	_test_eof381:  sm->cs = 381; goto _test_eof; 
	_test_eof382:  sm->cs = 382; goto _test_eof; 
	_test_eof383:  sm->cs = 383; goto _test_eof; 
	_test_eof384:  sm->cs = 384; goto _test_eof; 
	_test_eof385:  sm->cs = 385; goto _test_eof; 
	_test_eof386:  sm->cs = 386; goto _test_eof; 
	_test_eof387:  sm->cs = 387; goto _test_eof; 
	_test_eof388:  sm->cs = 388; goto _test_eof; 
	_test_eof389:  sm->cs = 389; goto _test_eof; 
	_test_eof390:  sm->cs = 390; goto _test_eof; 
	_test_eof391:  sm->cs = 391; goto _test_eof; 
	_test_eof392:  sm->cs = 392; goto _test_eof; 
	_test_eof393:  sm->cs = 393; goto _test_eof; 
	_test_eof394:  sm->cs = 394; goto _test_eof; 
	_test_eof395:  sm->cs = 395; goto _test_eof; 
	_test_eof396:  sm->cs = 396; goto _test_eof; 
	_test_eof397:  sm->cs = 397; goto _test_eof; 
	_test_eof398:  sm->cs = 398; goto _test_eof; 
	_test_eof399:  sm->cs = 399; goto _test_eof; 
	_test_eof400:  sm->cs = 400; goto _test_eof; 
	_test_eof401:  sm->cs = 401; goto _test_eof; 
	_test_eof402:  sm->cs = 402; goto _test_eof; 
	_test_eof403:  sm->cs = 403; goto _test_eof; 
	_test_eof404:  sm->cs = 404; goto _test_eof; 
	_test_eof405:  sm->cs = 405; goto _test_eof; 
	_test_eof406:  sm->cs = 406; goto _test_eof; 
	_test_eof407:  sm->cs = 407; goto _test_eof; 
	_test_eof408:  sm->cs = 408; goto _test_eof; 
	_test_eof409:  sm->cs = 409; goto _test_eof; 
	_test_eof410:  sm->cs = 410; goto _test_eof; 
	_test_eof411:  sm->cs = 411; goto _test_eof; 
	_test_eof412:  sm->cs = 412; goto _test_eof; 
	_test_eof413:  sm->cs = 413; goto _test_eof; 
	_test_eof414:  sm->cs = 414; goto _test_eof; 
	_test_eof415:  sm->cs = 415; goto _test_eof; 
	_test_eof416:  sm->cs = 416; goto _test_eof; 
	_test_eof417:  sm->cs = 417; goto _test_eof; 
	_test_eof418:  sm->cs = 418; goto _test_eof; 
	_test_eof419:  sm->cs = 419; goto _test_eof; 
	_test_eof420:  sm->cs = 420; goto _test_eof; 
	_test_eof421:  sm->cs = 421; goto _test_eof; 
	_test_eof422:  sm->cs = 422; goto _test_eof; 
	_test_eof423:  sm->cs = 423; goto _test_eof; 
	_test_eof424:  sm->cs = 424; goto _test_eof; 
	_test_eof425:  sm->cs = 425; goto _test_eof; 
	_test_eof426:  sm->cs = 426; goto _test_eof; 
	_test_eof427:  sm->cs = 427; goto _test_eof; 
	_test_eof428:  sm->cs = 428; goto _test_eof; 
	_test_eof429:  sm->cs = 429; goto _test_eof; 
	_test_eof430:  sm->cs = 430; goto _test_eof; 
	_test_eof431:  sm->cs = 431; goto _test_eof; 
	_test_eof432:  sm->cs = 432; goto _test_eof; 
	_test_eof433:  sm->cs = 433; goto _test_eof; 
	_test_eof434:  sm->cs = 434; goto _test_eof; 
	_test_eof435:  sm->cs = 435; goto _test_eof; 
	_test_eof436:  sm->cs = 436; goto _test_eof; 
	_test_eof437:  sm->cs = 437; goto _test_eof; 
	_test_eof438:  sm->cs = 438; goto _test_eof; 
	_test_eof439:  sm->cs = 439; goto _test_eof; 
	_test_eof440:  sm->cs = 440; goto _test_eof; 
	_test_eof441:  sm->cs = 441; goto _test_eof; 
	_test_eof442:  sm->cs = 442; goto _test_eof; 
	_test_eof443:  sm->cs = 443; goto _test_eof; 
	_test_eof444:  sm->cs = 444; goto _test_eof; 
	_test_eof445:  sm->cs = 445; goto _test_eof; 
	_test_eof446:  sm->cs = 446; goto _test_eof; 
	_test_eof447:  sm->cs = 447; goto _test_eof; 
	_test_eof448:  sm->cs = 448; goto _test_eof; 
	_test_eof449:  sm->cs = 449; goto _test_eof; 
	_test_eof450:  sm->cs = 450; goto _test_eof; 
	_test_eof451:  sm->cs = 451; goto _test_eof; 
	_test_eof452:  sm->cs = 452; goto _test_eof; 
	_test_eof453:  sm->cs = 453; goto _test_eof; 
	_test_eof454:  sm->cs = 454; goto _test_eof; 
	_test_eof455:  sm->cs = 455; goto _test_eof; 
	_test_eof456:  sm->cs = 456; goto _test_eof; 
	_test_eof457:  sm->cs = 457; goto _test_eof; 
	_test_eof458:  sm->cs = 458; goto _test_eof; 
	_test_eof459:  sm->cs = 459; goto _test_eof; 
	_test_eof460:  sm->cs = 460; goto _test_eof; 
	_test_eof461:  sm->cs = 461; goto _test_eof; 
	_test_eof462:  sm->cs = 462; goto _test_eof; 
	_test_eof463:  sm->cs = 463; goto _test_eof; 
	_test_eof464:  sm->cs = 464; goto _test_eof; 
	_test_eof465:  sm->cs = 465; goto _test_eof; 
	_test_eof466:  sm->cs = 466; goto _test_eof; 
	_test_eof467:  sm->cs = 467; goto _test_eof; 
	_test_eof468:  sm->cs = 468; goto _test_eof; 
	_test_eof578:  sm->cs = 578; goto _test_eof; 
	_test_eof579:  sm->cs = 579; goto _test_eof; 
	_test_eof469:  sm->cs = 469; goto _test_eof; 
	_test_eof470:  sm->cs = 470; goto _test_eof; 
	_test_eof471:  sm->cs = 471; goto _test_eof; 
	_test_eof472:  sm->cs = 472; goto _test_eof; 
	_test_eof473:  sm->cs = 473; goto _test_eof; 
	_test_eof474:  sm->cs = 474; goto _test_eof; 
	_test_eof475:  sm->cs = 475; goto _test_eof; 
	_test_eof580:  sm->cs = 580; goto _test_eof; 
	_test_eof581:  sm->cs = 581; goto _test_eof; 
	_test_eof582:  sm->cs = 582; goto _test_eof; 
	_test_eof583:  sm->cs = 583; goto _test_eof; 
	_test_eof476:  sm->cs = 476; goto _test_eof; 
	_test_eof477:  sm->cs = 477; goto _test_eof; 
	_test_eof478:  sm->cs = 478; goto _test_eof; 
	_test_eof479:  sm->cs = 479; goto _test_eof; 
	_test_eof480:  sm->cs = 480; goto _test_eof; 
	_test_eof584:  sm->cs = 584; goto _test_eof; 
	_test_eof585:  sm->cs = 585; goto _test_eof; 
	_test_eof481:  sm->cs = 481; goto _test_eof; 
	_test_eof482:  sm->cs = 482; goto _test_eof; 
	_test_eof483:  sm->cs = 483; goto _test_eof; 
	_test_eof484:  sm->cs = 484; goto _test_eof; 
	_test_eof485:  sm->cs = 485; goto _test_eof; 
	_test_eof486:  sm->cs = 486; goto _test_eof; 
	_test_eof487:  sm->cs = 487; goto _test_eof; 
	_test_eof488:  sm->cs = 488; goto _test_eof; 
	_test_eof489:  sm->cs = 489; goto _test_eof; 
	_test_eof490:  sm->cs = 490; goto _test_eof; 
	_test_eof491:  sm->cs = 491; goto _test_eof; 
	_test_eof492:  sm->cs = 492; goto _test_eof; 
	_test_eof493:  sm->cs = 493; goto _test_eof; 
	_test_eof494:  sm->cs = 494; goto _test_eof; 
	_test_eof495:  sm->cs = 495; goto _test_eof; 
	_test_eof496:  sm->cs = 496; goto _test_eof; 
	_test_eof497:  sm->cs = 497; goto _test_eof; 
	_test_eof498:  sm->cs = 498; goto _test_eof; 
	_test_eof499:  sm->cs = 499; goto _test_eof; 
	_test_eof500:  sm->cs = 500; goto _test_eof; 
	_test_eof501:  sm->cs = 501; goto _test_eof; 
	_test_eof502:  sm->cs = 502; goto _test_eof; 
	_test_eof503:  sm->cs = 503; goto _test_eof; 
	_test_eof504:  sm->cs = 504; goto _test_eof; 
	_test_eof505:  sm->cs = 505; goto _test_eof; 
	_test_eof506:  sm->cs = 506; goto _test_eof; 

	_test_eof: {}
	if ( ( sm->p) == ( sm->eof) )
	{
	switch (  sm->cs ) {
	case 508: goto tr0;
	case 0: goto tr0;
	case 509: goto tr586;
	case 510: goto tr586;
	case 1: goto tr2;
	case 511: goto tr587;
	case 512: goto tr587;
	case 2: goto tr2;
	case 513: goto tr586;
	case 3: goto tr2;
	case 514: goto tr590;
	case 515: goto tr586;
	case 4: goto tr2;
	case 5: goto tr2;
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
	case 516: goto tr597;
	case 17: goto tr2;
	case 18: goto tr2;
	case 19: goto tr2;
	case 20: goto tr2;
	case 21: goto tr2;
	case 517: goto tr598;
	case 22: goto tr2;
	case 23: goto tr2;
	case 24: goto tr2;
	case 25: goto tr2;
	case 26: goto tr2;
	case 27: goto tr2;
	case 28: goto tr2;
	case 29: goto tr2;
	case 30: goto tr2;
	case 31: goto tr2;
	case 32: goto tr2;
	case 33: goto tr2;
	case 34: goto tr2;
	case 35: goto tr2;
	case 36: goto tr2;
	case 37: goto tr2;
	case 38: goto tr2;
	case 39: goto tr2;
	case 518: goto tr599;
	case 519: goto tr601;
	case 40: goto tr2;
	case 41: goto tr2;
	case 520: goto tr602;
	case 521: goto tr604;
	case 42: goto tr2;
	case 43: goto tr2;
	case 44: goto tr2;
	case 45: goto tr2;
	case 46: goto tr2;
	case 47: goto tr2;
	case 48: goto tr2;
	case 522: goto tr605;
	case 49: goto tr2;
	case 50: goto tr2;
	case 51: goto tr2;
	case 52: goto tr2;
	case 53: goto tr2;
	case 523: goto tr586;
	case 525: goto tr609;
	case 54: goto tr64;
	case 55: goto tr64;
	case 56: goto tr64;
	case 57: goto tr64;
	case 58: goto tr64;
	case 59: goto tr64;
	case 60: goto tr64;
	case 61: goto tr64;
	case 62: goto tr64;
	case 63: goto tr64;
	case 64: goto tr64;
	case 65: goto tr64;
	case 66: goto tr64;
	case 67: goto tr64;
	case 68: goto tr64;
	case 69: goto tr64;
	case 70: goto tr64;
	case 71: goto tr64;
	case 72: goto tr64;
	case 73: goto tr64;
	case 74: goto tr64;
	case 75: goto tr64;
	case 76: goto tr64;
	case 527: goto tr639;
	case 528: goto tr644;
	case 77: goto tr96;
	case 78: goto tr98;
	case 79: goto tr98;
	case 80: goto tr98;
	case 81: goto tr96;
	case 82: goto tr96;
	case 83: goto tr96;
	case 84: goto tr96;
	case 85: goto tr96;
	case 86: goto tr96;
	case 87: goto tr96;
	case 88: goto tr96;
	case 89: goto tr96;
	case 90: goto tr112;
	case 91: goto tr112;
	case 529: goto tr646;
	case 530: goto tr646;
	case 92: goto tr112;
	case 93: goto tr112;
	case 531: goto tr648;
	case 94: goto tr112;
	case 95: goto tr112;
	case 96: goto tr96;
	case 97: goto tr96;
	case 98: goto tr96;
	case 99: goto tr96;
	case 100: goto tr96;
	case 532: goto tr650;
	case 101: goto tr112;
	case 102: goto tr96;
	case 103: goto tr96;
	case 104: goto tr96;
	case 105: goto tr96;
	case 106: goto tr96;
	case 107: goto tr96;
	case 533: goto tr651;
	case 534: goto tr652;
	case 535: goto tr653;
	case 108: goto tr134;
	case 109: goto tr134;
	case 110: goto tr134;
	case 111: goto tr134;
	case 536: goto tr655;
	case 112: goto tr134;
	case 113: goto tr134;
	case 114: goto tr134;
	case 115: goto tr134;
	case 116: goto tr134;
	case 117: goto tr134;
	case 118: goto tr134;
	case 119: goto tr134;
	case 120: goto tr134;
	case 121: goto tr134;
	case 122: goto tr134;
	case 123: goto tr134;
	case 124: goto tr134;
	case 125: goto tr134;
	case 126: goto tr134;
	case 127: goto tr134;
	case 128: goto tr134;
	case 537: goto tr653;
	case 129: goto tr134;
	case 130: goto tr134;
	case 131: goto tr134;
	case 132: goto tr134;
	case 133: goto tr134;
	case 134: goto tr134;
	case 135: goto tr134;
	case 136: goto tr134;
	case 137: goto tr134;
	case 538: goto tr653;
	case 138: goto tr134;
	case 139: goto tr134;
	case 140: goto tr134;
	case 141: goto tr134;
	case 142: goto tr134;
	case 143: goto tr134;
	case 539: goto tr659;
	case 144: goto tr134;
	case 145: goto tr134;
	case 146: goto tr134;
	case 147: goto tr134;
	case 148: goto tr134;
	case 149: goto tr134;
	case 150: goto tr134;
	case 540: goto tr661;
	case 541: goto tr653;
	case 151: goto tr134;
	case 152: goto tr134;
	case 153: goto tr134;
	case 154: goto tr134;
	case 542: goto tr666;
	case 155: goto tr134;
	case 156: goto tr134;
	case 157: goto tr134;
	case 158: goto tr134;
	case 159: goto tr134;
	case 543: goto tr668;
	case 160: goto tr134;
	case 161: goto tr134;
	case 162: goto tr134;
	case 163: goto tr134;
	case 544: goto tr670;
	case 545: goto tr653;
	case 164: goto tr134;
	case 165: goto tr134;
	case 166: goto tr134;
	case 167: goto tr134;
	case 168: goto tr134;
	case 169: goto tr134;
	case 170: goto tr134;
	case 171: goto tr134;
	case 546: goto tr673;
	case 547: goto tr653;
	case 172: goto tr134;
	case 173: goto tr134;
	case 174: goto tr134;
	case 175: goto tr134;
	case 176: goto tr134;
	case 548: goto tr677;
	case 177: goto tr134;
	case 178: goto tr134;
	case 179: goto tr134;
	case 180: goto tr134;
	case 181: goto tr134;
	case 182: goto tr134;
	case 549: goto tr679;
	case 550: goto tr653;
	case 183: goto tr134;
	case 184: goto tr134;
	case 185: goto tr134;
	case 186: goto tr134;
	case 187: goto tr134;
	case 188: goto tr134;
	case 551: goto tr682;
	case 189: goto tr134;
	case 552: goto tr653;
	case 190: goto tr134;
	case 191: goto tr134;
	case 192: goto tr134;
	case 193: goto tr134;
	case 194: goto tr134;
	case 195: goto tr134;
	case 196: goto tr134;
	case 197: goto tr134;
	case 198: goto tr134;
	case 199: goto tr134;
	case 200: goto tr134;
	case 201: goto tr134;
	case 553: goto tr684;
	case 554: goto tr653;
	case 202: goto tr134;
	case 203: goto tr134;
	case 204: goto tr134;
	case 205: goto tr134;
	case 206: goto tr134;
	case 207: goto tr134;
	case 208: goto tr134;
	case 209: goto tr134;
	case 210: goto tr134;
	case 211: goto tr134;
	case 212: goto tr134;
	case 555: goto tr687;
	case 556: goto tr653;
	case 213: goto tr134;
	case 214: goto tr134;
	case 215: goto tr134;
	case 216: goto tr134;
	case 217: goto tr134;
	case 557: goto tr690;
	case 558: goto tr653;
	case 218: goto tr134;
	case 219: goto tr134;
	case 220: goto tr134;
	case 221: goto tr134;
	case 222: goto tr134;
	case 559: goto tr693;
	case 223: goto tr134;
	case 224: goto tr134;
	case 225: goto tr134;
	case 226: goto tr134;
	case 560: goto tr695;
	case 227: goto tr134;
	case 228: goto tr134;
	case 229: goto tr134;
	case 230: goto tr134;
	case 231: goto tr134;
	case 232: goto tr134;
	case 233: goto tr134;
	case 234: goto tr134;
	case 235: goto tr134;
	case 561: goto tr697;
	case 562: goto tr653;
	case 236: goto tr134;
	case 237: goto tr134;
	case 238: goto tr134;
	case 239: goto tr134;
	case 240: goto tr134;
	case 241: goto tr134;
	case 242: goto tr134;
	case 563: goto tr700;
	case 564: goto tr653;
	case 243: goto tr134;
	case 244: goto tr134;
	case 245: goto tr134;
	case 246: goto tr134;
	case 565: goto tr703;
	case 566: goto tr653;
	case 247: goto tr134;
	case 248: goto tr134;
	case 249: goto tr134;
	case 250: goto tr134;
	case 251: goto tr134;
	case 252: goto tr134;
	case 253: goto tr134;
	case 254: goto tr134;
	case 255: goto tr134;
	case 256: goto tr134;
	case 567: goto tr709;
	case 257: goto tr134;
	case 258: goto tr134;
	case 259: goto tr134;
	case 260: goto tr134;
	case 261: goto tr134;
	case 262: goto tr134;
	case 263: goto tr134;
	case 264: goto tr134;
	case 265: goto tr134;
	case 266: goto tr134;
	case 267: goto tr134;
	case 268: goto tr134;
	case 269: goto tr134;
	case 270: goto tr134;
	case 568: goto tr711;
	case 271: goto tr134;
	case 272: goto tr134;
	case 273: goto tr134;
	case 274: goto tr134;
	case 275: goto tr134;
	case 276: goto tr134;
	case 277: goto tr134;
	case 569: goto tr713;
	case 278: goto tr134;
	case 279: goto tr134;
	case 280: goto tr134;
	case 281: goto tr134;
	case 282: goto tr134;
	case 283: goto tr134;
	case 570: goto tr715;
	case 571: goto tr653;
	case 284: goto tr134;
	case 285: goto tr134;
	case 286: goto tr134;
	case 287: goto tr134;
	case 288: goto tr134;
	case 572: goto tr718;
	case 573: goto tr653;
	case 289: goto tr134;
	case 290: goto tr134;
	case 291: goto tr134;
	case 292: goto tr134;
	case 293: goto tr134;
	case 574: goto tr721;
	case 575: goto tr653;
	case 294: goto tr134;
	case 295: goto tr134;
	case 296: goto tr134;
	case 297: goto tr134;
	case 298: goto tr134;
	case 299: goto tr134;
	case 300: goto tr134;
	case 301: goto tr134;
	case 576: goto tr734;
	case 302: goto tr134;
	case 303: goto tr134;
	case 304: goto tr134;
	case 305: goto tr134;
	case 306: goto tr134;
	case 307: goto tr134;
	case 308: goto tr134;
	case 309: goto tr134;
	case 310: goto tr134;
	case 311: goto tr134;
	case 312: goto tr134;
	case 313: goto tr134;
	case 314: goto tr134;
	case 577: goto tr735;
	case 315: goto tr134;
	case 316: goto tr134;
	case 317: goto tr134;
	case 318: goto tr134;
	case 319: goto tr134;
	case 320: goto tr134;
	case 321: goto tr134;
	case 322: goto tr134;
	case 323: goto tr134;
	case 324: goto tr134;
	case 325: goto tr134;
	case 326: goto tr134;
	case 327: goto tr134;
	case 328: goto tr134;
	case 329: goto tr134;
	case 330: goto tr134;
	case 331: goto tr134;
	case 332: goto tr134;
	case 333: goto tr134;
	case 334: goto tr134;
	case 335: goto tr134;
	case 336: goto tr134;
	case 337: goto tr134;
	case 338: goto tr134;
	case 339: goto tr134;
	case 340: goto tr134;
	case 341: goto tr134;
	case 342: goto tr134;
	case 343: goto tr134;
	case 344: goto tr134;
	case 345: goto tr134;
	case 346: goto tr134;
	case 347: goto tr134;
	case 348: goto tr134;
	case 349: goto tr134;
	case 350: goto tr134;
	case 351: goto tr134;
	case 352: goto tr134;
	case 353: goto tr134;
	case 354: goto tr134;
	case 355: goto tr134;
	case 356: goto tr134;
	case 357: goto tr134;
	case 358: goto tr134;
	case 359: goto tr134;
	case 360: goto tr134;
	case 361: goto tr134;
	case 362: goto tr134;
	case 363: goto tr134;
	case 364: goto tr134;
	case 365: goto tr134;
	case 366: goto tr134;
	case 367: goto tr134;
	case 368: goto tr134;
	case 369: goto tr134;
	case 370: goto tr134;
	case 371: goto tr134;
	case 372: goto tr134;
	case 373: goto tr134;
	case 374: goto tr134;
	case 375: goto tr134;
	case 376: goto tr134;
	case 377: goto tr134;
	case 378: goto tr134;
	case 379: goto tr134;
	case 380: goto tr134;
	case 381: goto tr134;
	case 382: goto tr134;
	case 383: goto tr134;
	case 384: goto tr134;
	case 385: goto tr134;
	case 386: goto tr134;
	case 387: goto tr134;
	case 388: goto tr134;
	case 389: goto tr134;
	case 390: goto tr134;
	case 391: goto tr134;
	case 392: goto tr134;
	case 393: goto tr134;
	case 394: goto tr134;
	case 395: goto tr134;
	case 396: goto tr134;
	case 397: goto tr134;
	case 398: goto tr134;
	case 399: goto tr134;
	case 400: goto tr134;
	case 401: goto tr134;
	case 402: goto tr134;
	case 403: goto tr134;
	case 404: goto tr134;
	case 405: goto tr134;
	case 406: goto tr134;
	case 407: goto tr134;
	case 408: goto tr134;
	case 409: goto tr134;
	case 410: goto tr134;
	case 411: goto tr134;
	case 412: goto tr134;
	case 413: goto tr134;
	case 414: goto tr134;
	case 415: goto tr134;
	case 416: goto tr134;
	case 417: goto tr134;
	case 418: goto tr134;
	case 419: goto tr134;
	case 420: goto tr134;
	case 421: goto tr134;
	case 422: goto tr134;
	case 423: goto tr134;
	case 424: goto tr134;
	case 425: goto tr134;
	case 426: goto tr134;
	case 427: goto tr134;
	case 428: goto tr134;
	case 429: goto tr134;
	case 430: goto tr134;
	case 431: goto tr134;
	case 432: goto tr134;
	case 433: goto tr134;
	case 434: goto tr134;
	case 435: goto tr134;
	case 436: goto tr134;
	case 437: goto tr134;
	case 438: goto tr134;
	case 439: goto tr134;
	case 440: goto tr134;
	case 441: goto tr134;
	case 442: goto tr134;
	case 443: goto tr134;
	case 444: goto tr134;
	case 445: goto tr134;
	case 446: goto tr134;
	case 447: goto tr134;
	case 448: goto tr134;
	case 449: goto tr134;
	case 450: goto tr134;
	case 451: goto tr134;
	case 452: goto tr134;
	case 453: goto tr134;
	case 454: goto tr134;
	case 455: goto tr134;
	case 456: goto tr134;
	case 457: goto tr134;
	case 458: goto tr134;
	case 459: goto tr134;
	case 460: goto tr134;
	case 461: goto tr134;
	case 462: goto tr134;
	case 463: goto tr134;
	case 464: goto tr134;
	case 465: goto tr134;
	case 466: goto tr134;
	case 467: goto tr134;
	case 468: goto tr134;
	case 578: goto tr653;
	case 579: goto tr653;
	case 469: goto tr134;
	case 470: goto tr134;
	case 471: goto tr134;
	case 472: goto tr134;
	case 473: goto tr134;
	case 474: goto tr134;
	case 475: goto tr134;
	case 581: goto tr741;
	case 583: goto tr745;
	case 476: goto tr537;
	case 477: goto tr537;
	case 478: goto tr537;
	case 479: goto tr537;
	case 480: goto tr537;
	case 585: goto tr749;
	case 481: goto tr543;
	case 482: goto tr543;
	case 483: goto tr543;
	case 484: goto tr543;
	case 485: goto tr543;
	case 486: goto tr543;
	case 487: goto tr543;
	case 488: goto tr543;
	case 489: goto tr543;
	case 490: goto tr543;
	case 491: goto tr543;
	case 492: goto tr543;
	case 493: goto tr543;
	case 494: goto tr543;
	case 495: goto tr543;
	case 496: goto tr543;
	case 497: goto tr543;
	case 498: goto tr543;
	case 499: goto tr543;
	case 500: goto tr543;
	case 501: goto tr543;
	case 502: goto tr543;
	case 503: goto tr543;
	case 504: goto tr543;
	case 505: goto tr543;
	case 506: goto tr543;
	}
	}

	}

#line 1077 "ext/dtext/dtext.cpp.rl"

  sm->dstack_close_all();

  return DTextResult { sm->output, sm->posts };
}

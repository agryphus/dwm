/* See LICENSE file for copyright and license details. */


/* appearance */
static const unsigned int borderpx       = 3;   /* border pixel of windows */
static const unsigned int snap           = 32;  /* snap pixel */
static const unsigned int gappih         = 10;  /* horiz inner gap between windows */
static const unsigned int gappiv         = 10;  /* vert inner gap between windows */
static const unsigned int gappoh         = 10;  /* horiz outer gap between windows and screen edge */
static const unsigned int gappov         = 10;  /* vert outer gap between windows and screen edge */
static const int smartgaps_fact          = 1;   /* gap factor when there is only one client; 0 = no gaps, 3 = 3x outer gaps */
static const int showbar                 = 1;   /* 0 means no bar */
static const int topbar                  = 1;   /* 0 means bottom bar */

/* Status is to be shown on: -1 (all monitors), 0 (a specific monitor by index), 'A' (active monitor) */
static const int statusmon               = -1;

static const unsigned int ulinepad = 5;         /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke  = borderpx;     /* thickness / height of the underline */
static const unsigned int ulinevoffset = 0;     /* how far above the bottom of the bar the line should appear */
static const int ulineall = 0;                  /* 1 to show underline on all tags, 0 for just the active ones */

/* Indicators: see patch/bar_indicators.h for options */
static int tagindicatortype              = INDICATOR_NONE;
static int tiledindicatortype            = INDICATOR_NONE;
static int floatindicatortype            = INDICATOR_TOP_LEFT_SQUARE;

static char *fonts[]          = { 
    "FiraCode Nerd Font:size=13",
    "NotoColorEmoji:pixelsize=13:antialias=true:autohint=true"
};

static const char dmenufont[]            = "monospace:size=10";

static char c000000[]                    = "#000000"; // placeholder value
static char black[]                      = "#000000";
static char base_color[]                 = "#227799";

static char normfgcolor[]                = "#bbbbbb";
static char hidfgcolor[]                 = "#777777";
static char normbgcolor[]                = "#222222";
static char normbordercolor[]            = "#444444";
static char normfloatcolor[]             = "#db8fd9";

static char selfgcolor[]                 = "#eeeeee";

static char hidselfgcolor[]              = "#227799";
static char hidnormbgcolor[]             = "#222222";
static char hidselbgcolor[]              = "#222222";

static char urgfgcolor[]                 = "#bbbbbb";
static char urgbgcolor[]                 = "#222222";
static char urgbordercolor[]             = "#ff0000";
static char urgfloatcolor[]              = "#db8fd9";

static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = 0xd0;
static const unsigned int alphas[][3] = {
    /*                       fg      bg        border     */
    [SchemeNorm]         = { OPAQUE, baralpha, borderalpha },
    [SchemeSel]          = { OPAQUE, baralpha, borderalpha },
    [SchemeTitleNorm]    = { OPAQUE, baralpha, borderalpha },
    [SchemeTitleSel]     = { OPAQUE, baralpha, borderalpha },
    [SchemeTagsNorm]     = { OPAQUE, baralpha, borderalpha },
    [SchemeTagsSel]      = { OPAQUE, baralpha, borderalpha },
    [SchemeHidNorm]      = { OPAQUE, baralpha, borderalpha },
    [SchemeHidSel]       = { OPAQUE, baralpha, borderalpha },
    [SchemeUrg]          = { OPAQUE, baralpha, borderalpha },
};


static char *colors[][ColCount] = {
    /*                       fg                bg                border                float */
    [SchemeNorm]         = { normfgcolor,      black,            normbordercolor,      normfloatcolor },
    [SchemeTitleNorm]    = { hidfgcolor,       black,            normbordercolor,      normfloatcolor },
    [SchemeTagsNorm]     = { normfgcolor,      black,            normbordercolor,      normfloatcolor },

    [SchemeSel]          = { selfgcolor,       black,            base_color,           base_color },
    [SchemeTitleSel]     = { selfgcolor,       black,            base_color,           base_color },
    [SchemeTagsSel]      = { normfgcolor,      black,            base_color,           base_color }, 

    [SchemeHidNorm]      = { base_color,       black,            c000000,              c000000 },
    [SchemeHidSel]       = { hidselfgcolor,    black,            c000000,              c000000 },
    [SchemeUrg]          = { urgfgcolor,       black,            urgbordercolor,       urgfloatcolor },
};

/* Bar rules allow you to configure what is shown where on the bar, as well as
 * introducing your own bar modules.
 *
 *    monitor:
 *      -1  show on all monitors
 *       0  show on monitor 0
 *      'A' show on active monitor (i.e. focused / selected) (or just -1 for active?)
 *    bar - bar index, 0 is default, 1 is extrabar
 *    alignment - how the module is aligned compared to other modules
 *    widthfunc, drawfunc, clickfunc - providing bar module width, draw and click functions
 *    name - does nothing, intended for visual clue and for logging / debugging
 */
static const BarRule barrules[] = {
    /* monitor   bar    alignment         widthfunc                 drawfunc                clickfunc                hoverfunc                name */
    { -1,        0,     BAR_ALIGN_LEFT,   width_tags,               draw_tags,              click_tags,              hover_tags,              "tags" },
    { -1,        0,     BAR_ALIGN_LEFT,   width_ltsymbol,           draw_ltsymbol,          click_ltsymbol,          NULL,                    "layout" },
    { statusmon, 0,     BAR_ALIGN_RIGHT,  width_status,             draw_status,            click_statuscmd,         NULL,                    "status" },
    { -1,        0,     BAR_ALIGN_NONE,   width_wintitle,           draw_wintitle,          click_wintitle,          NULL,                    "wintitle" },
};

// Nametag Patch
#define NAMETAG_FORMAT "%s"
/* The maximum amount of bytes reserved for each tag text. */
#define MAX_TAGLEN 16
/* The command to run (via popen). This can be tailored by adding a prompt, passing other command
 * line arguments or providing name options. Optionally you can use other dmenu like alternatives
 * like rofi -dmenu. */
#define NAMETAG_COMMAND "dmenuunicode output < /dev/null"

/* Tags
 * In a traditional dwm the number of tags in use can be changed simply by changing the number
 * of strings in the tags array. This build does things a bit different which has some added
 * benefits. If you need to change the number of tags here then change the NUMTAGS macro in dwm.c.
 *
 * Examples:
 *
 *  1) static char *tagicons[][NUMTAGS*2] = {
 *         [DEFAULT_TAGS] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H", "I" },
 *     }
 *
 *  2) static char *tagicons[][1] = {
 *         [DEFAULT_TAGS] = { "•" },
 *     }
 *
 * The first example would result in the tags on the first monitor to be 1 through 9, while the
 * tags for the second monitor would be named A through I. A third monitor would start again at
 * 1 through 9 while the tags on a fourth monitor would also be named A through I. Note the tags
 * count of NUMTAGS*2 in the array initialiser which defines how many tag text / icon exists in
 * the array. This can be changed to *3 to add separate icons for a third monitor.
 *
 * For the second example each tag would be represented as a bullet point. Both cases work the
 * same from a technical standpoint - the icon index is derived from the tag index and the monitor
 * index. If the icon index is is greater than the number of tag icons then it will wrap around
 * until it an icon matches. Similarly if there are two tag icons then it would alternate between
 * them. This works seamlessly with alternative tags and alttagsdecoration patches.
 */
static char tagicons[][NUMTAGS][MAX_TAGLEN] =
{
    [DEFAULT_TAGS]        = { "1", "2", "3", "4", "5", "6", "7", "8", "9" },
    [ALTERNATIVE_TAGS]    = { "A", "B", "C", "D", "E", "F", "G", "H", "I" },
    [ALT_TAGS_DECORATION] = { "<1>", "<2>", "<3>", "<4>", "<5>", "<6>", "<7>", "<8>", "<9>" },
};

/* There are two options when it comes to per-client rules:
 *  - a typical struct table or
 *  - using the RULE macro
 *
 * A traditional struct table looks like this:
 *    // class      instance  title  wintype  tags mask  isfloating  monitor
 *    { "Gimp",     NULL,     NULL,  NULL,    1 << 4,    0,          -1 },
 *    { "Firefox",  NULL,     NULL,  NULL,    1 << 7,    0,          -1 },
 *
 * The RULE macro has the default values set for each field allowing you to only
 * specify the values that are relevant for your rule, e.g.
 *
 *    RULE(.class = "Gimp", .tags = 1 << 4)
 *    RULE(.class = "Firefox", .tags = 1 << 7)
 *
 * Refer to the Rule struct definition for the list of available fields depending on
 * the patches you enable.
 */
static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     *  WM_WINDOW_ROLE(STRING) = role
     *  _NET_WM_WINDOW_TYPE(ATOM) = wintype
     */
    RULE(.wintype = WTYPE "DIALOG", .isfloating = 1)
    RULE(.wintype = WTYPE "UTILITY", .isfloating = 1)
    RULE(.wintype = WTYPE "TOOLBAR", .isfloating = 1)
    RULE(.wintype = WTYPE "SPLASH", .isfloating = 1)
    RULE(.class = "Gimp", .tags = 1 << 4)
    RULE(.class = "Firefox", .tags = 1 << 7)
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "><>",      NULL },    /* no layout function means floating behavior */
    { "[M]",      monocle },
    { "TTT",      bstack },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

// Stacker Patch
#define STACKKEYS(MOD,ACTION) \
    { MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
    { MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
    { MOD, XK_v,     ACTION##stack, {.i = 0 } }, \

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {
    "dmenu_run",
    "-m", dmenumon,
    "-fn", dmenufont,
    "-nb", normbgcolor,
    "-nf", normfgcolor,
    "-sf", selfgcolor,
    NULL
};
static const char *termcmd[]  = { "st", NULL };

/* This defines the name of the executable that handles the bar (used for signalling purposes) */
#define STATUSBAR "dwmblocks"
/* test the above with: xsetroot -name "$(printf '\x01Volume |\x02 CPU |\x03 Battery')" */
static const char *statuscmd[] = { "/bin/sh", "-c", NULL, NULL };

static const Key keys[] = {
    /* modifier                     key            function                argument */

    // Spawning the terminal.  I use sxhkd for the rest of my hotkey needs, but this is defined here in case
    // that program dies.
    { MODKEY|ShiftMask,             XK_Return,     spawn,                  {.v = termcmd } },

    // Nametag Patch
    { MODKEY|ShiftMask,             XK_n,          nametag,                {0} },

    // Vanity Gaps Patch
    { MODKEY,                       XK_z,          incrgaps,               {.i = +3 } },
    { MODKEY,                       XK_x,          incrgaps,               {.i = -3 } },
    { MODKEY,                       XK_a,          togglegaps,             {0} },
    { MODKEY|ShiftMask,             XK_a,          defaultgaps,            {0} },

    // Fullscreen Patch
    { MODKEY,                       XK_f,          togglefullscreen,       {0} },

    // Stacker Patch
    STACKKEYS(MODKEY,                              focus)
    STACKKEYS(MODKEY|ShiftMask,                    push)

    // Layouts
    { MODKEY,                       XK_t,          setlayout,              {.v = &layouts[0]} }, // Tiled (default)
    { MODKEY|ShiftMask,             XK_f,          setlayout,              {.v = &layouts[1]} }, // Floating
    { MODKEY,                       XK_m,          setlayout,              {.v = &layouts[2]} }, // Monacle
    { MODKEY|ShiftMask,             XK_t,          setlayout,              {.v = &layouts[3]} }, // BStack

    // Monitor switching
    { MODKEY,                       XK_Left,       focusmon,               {.i = -1 } },
    { MODKEY,                       XK_comma,      focusmon,               {.i = -1 } },
    { MODKEY,                       XK_Right,      focusmon,               {.i = +1 } },
    { MODKEY,                       XK_period,     focusmon,               {.i = +1 } },
    { MODKEY|ShiftMask,             XK_Left,       tagmon,                 {.i = -1 } }, 
    { MODKEY|ShiftMask,             XK_comma,      tagmon,                 {.i = -1 } }, 
    { MODKEY|ShiftMask,             XK_Right,      tagmon,                 {.i = +1 } },
    { MODKEY|ShiftMask,             XK_period,     tagmon,                 {.i = +1 } },

    { MODKEY,                       XK_b,          togglebar,              {0} },
    { MODKEY,                       XK_0,          view,                   {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,          tag,                    {.ui = ~0 } },
    { MODKEY,                       XK_space,      setlayout,              {0} },
    { MODKEY|ShiftMask,             XK_space,      togglefloating,         {0} },
    { MODKEY,                       XK_Tab,        view,                   {0} },
    { MODKEY,                       XK_o,          incnmaster,             {.i = +1 } },
    { MODKEY|ShiftMask,             XK_o,          incnmaster,             {.i = -1 } },
    { MODKEY,                       XK_space,      zoom,                   {0} },
    { MODKEY|ShiftMask,             XK_h,          setmfact,               {.f = -0.05} },
    { MODKEY|ShiftMask,             XK_l,          setmfact,               {.f = +0.05} },

    { MODKEY,                       XK_q,          killclient,             {0} },
    { MODKEY|ShiftMask,             XK_q,          quit,                   {1} }, // SIGTERM Patch.  Restart dwm nicely
    { MODKEY|ControlMask|ShiftMask, XK_q,          quit,                   {0} },

    TAGKEYS(                        XK_1,                                  0)
    TAGKEYS(                        XK_2,                                  1)
    TAGKEYS(                        XK_3,                                  2)
    TAGKEYS(                        XK_4,                                  3)
    TAGKEYS(                        XK_5,                                  4)
    TAGKEYS(                        XK_6,                                  5)
    TAGKEYS(                        XK_7,                                  6)
    TAGKEYS(                        XK_8,                                  7)
    TAGKEYS(                        XK_9,                                  8)
};


/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask           button          function        argument */
    { ClkLtSymbol,          0,                   Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,                   Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,                   Button2,        zoom,           {0} },
    { ClkStatusText,        0,                   Button1,        sigstatusbar,   {.i = 1 } },
    { ClkStatusText,        0,                   Button2,        sigstatusbar,   {.i = 2 } },
    { ClkStatusText,        0,                   Button3,        sigstatusbar,   {.i = 3 } },
    { ClkStatusText,        0,                   Button1,        spawn,          {.v = statuscmd } },
    { ClkStatusText,        0,                   Button2,        spawn,          {.v = statuscmd } },
    { ClkStatusText,        0,                   Button3,        spawn,          {.v = statuscmd } },
    { ClkStatusText,        0,                   Button2,        spawn,          {.v = termcmd } },
    { ClkClientWin,         MODKEY,              Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,              Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,              Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,                   Button1,        view,           {0} },
    { ClkTagBar,            0,                   Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,              Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,              Button3,        toggletag,      {0} },
};


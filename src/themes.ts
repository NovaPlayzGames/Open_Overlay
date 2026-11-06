// ---------------------------------------------------------------------------
// Theme system — CSS-variable driven app themes.
// Each theme sets the --oo-* custom properties consumed by index.css @theme
// tokens (bg-app, bg-panel, text-dim, border-line, bg-accent, ...).
// ---------------------------------------------------------------------------

export interface AppTheme {
  id: string;
  label: string;
  /** 'dark' or 'light' — drives a few conditional bits of UI chrome */
  mode: 'dark' | 'light';
  vars: Record<string, string>;
}

export const THEMES: AppTheme[] = [
  {
    id: 'midnight',
    label: 'Midnight',
    mode: 'dark',
    vars: {
      '--oo-app': '#050505', '--oo-panel': '#0E0E11', '--oo-chrome': '#0A0A0D',
      '--oo-popover': '#18181B', '--oo-input': '#222226',
      '--oo-hover': 'rgba(255,255,255,0.05)', '--oo-active': 'rgba(255,255,255,0.10)',
      '--oo-border': 'rgba(255,255,255,0.06)', '--oo-border2': 'rgba(255,255,255,0.15)',
      '--oo-accent': '#3b82f6', '--oo-accent2': '#8b5cf6', '--oo-onaccent': '#ffffff',
      '--oo-text': 'rgba(255,255,255,0.92)', '--oo-dim': 'rgba(255,255,255,0.45)', '--oo-mute': 'rgba(255,255,255,0.22)',
      '--oo-canvas-dot': 'rgba(255,255,255,0.04)', '--oo-checker': '#141414',
      '--oo-gridline': 'rgba(255,255,255,0.05)',
    },
  },
  {
    id: 'obsidian',
    label: 'Obsidian',
    mode: 'dark',
    vars: {
      '--oo-app': '#000000', '--oo-panel': '#0a0a0a', '--oo-chrome': '#060606',
      '--oo-popover': '#111113', '--oo-input': '#1c1c1f',
      '--oo-hover': 'rgba(255,255,255,0.06)', '--oo-active': 'rgba(255,255,255,0.11)',
      '--oo-border': 'rgba(255,255,255,0.07)', '--oo-border2': 'rgba(255,255,255,0.16)',
      '--oo-accent': '#a3a3a3', '--oo-accent2': '#d4d4d4', '--oo-onaccent': '#0a0a0a',
      '--oo-text': 'rgba(255,255,255,0.92)', '--oo-dim': 'rgba(255,255,255,0.45)', '--oo-mute': 'rgba(255,255,255,0.22)',
      '--oo-canvas-dot': 'rgba(255,255,255,0.05)', '--oo-checker': '#101010',
      '--oo-gridline': 'rgba(255,255,255,0.05)',
    },
  },
  {
    id: 'tokyo',
    label: 'Tokyo Night',
    mode: 'dark',
    vars: {
      '--oo-app': '#14151e', '--oo-panel': '#1a1b26', '--oo-chrome': '#16161e',
      '--oo-popover': '#1f2335', '--oo-input': '#24283b',
      '--oo-hover': 'rgba(122,162,247,0.07)', '--oo-active': 'rgba(122,162,247,0.14)',
      '--oo-border': 'rgba(122,162,247,0.08)', '--oo-border2': 'rgba(122,162,247,0.20)',
      '--oo-accent': '#7aa2f7', '--oo-accent2': '#bb9af7', '--oo-onaccent': '#1a1b26',
      '--oo-text': 'rgba(192,202,245,0.95)', '--oo-dim': 'rgba(169,177,214,0.55)', '--oo-mute': 'rgba(86,95,137,0.75)',
      '--oo-canvas-dot': 'rgba(122,162,247,0.05)', '--oo-checker': '#191a25',
      '--oo-gridline': 'rgba(122,162,247,0.06)',
    },
  },
  {
    id: 'dracula',
    label: 'Dracula',
    mode: 'dark',
    vars: {
      '--oo-app': '#1e1f29', '--oo-panel': '#282a36', '--oo-chrome': '#21222c',
      '--oo-popover': '#2f3140', '--oo-input': '#343746',
      '--oo-hover': 'rgba(255,121,198,0.06)', '--oo-active': 'rgba(255,121,198,0.12)',
      '--oo-border': 'rgba(98,114,164,0.25)', '--oo-border2': 'rgba(98,114,164,0.45)',
      '--oo-accent': '#ff79c6', '--oo-accent2': '#bd93f9', '--oo-onaccent': '#282a36',
      '--oo-text': 'rgba(248,248,242,0.95)', '--oo-dim': 'rgba(248,248,242,0.50)', '--oo-mute': 'rgba(98,114,164,0.85)',
      '--oo-canvas-dot': 'rgba(248,248,242,0.04)', '--oo-checker': '#232433',
      '--oo-gridline': 'rgba(98,114,164,0.15)',
    },
  },
  {
    id: 'nord',
    label: 'Nord',
    mode: 'dark',
    vars: {
      '--oo-app': '#242933', '--oo-panel': '#2e3440', '--oo-chrome': '#272c36',
      '--oo-popover': '#353b49', '--oo-input': '#3b4252',
      '--oo-hover': 'rgba(136,192,208,0.07)', '--oo-active': 'rgba(136,192,208,0.13)',
      '--oo-border': 'rgba(216,222,233,0.07)', '--oo-border2': 'rgba(216,222,233,0.16)',
      '--oo-accent': '#88c0d0', '--oo-accent2': '#81a1c1', '--oo-onaccent': '#2e3440',
      '--oo-text': 'rgba(236,239,244,0.95)', '--oo-dim': 'rgba(216,222,233,0.50)', '--oo-mute': 'rgba(216,222,233,0.25)',
      '--oo-canvas-dot': 'rgba(216,222,233,0.04)', '--oo-checker': '#2b303b',
      '--oo-gridline': 'rgba(136,192,208,0.07)',
    },
  },
  {
    id: 'catppuccin',
    label: 'Catppuccin',
    mode: 'dark',
    vars: {
      '--oo-app': '#181825', '--oo-panel': '#1e1e2e', '--oo-chrome': '#181825',
      '--oo-popover': '#262637', '--oo-input': '#313244',
      '--oo-hover': 'rgba(203,166,247,0.07)', '--oo-active': 'rgba(203,166,247,0.13)',
      '--oo-border': 'rgba(205,214,244,0.08)', '--oo-border2': 'rgba(205,214,244,0.16)',
      '--oo-accent': '#cba6f7', '--oo-accent2': '#89b4fa', '--oo-onaccent': '#1e1e2e',
      '--oo-text': 'rgba(205,214,244,0.95)', '--oo-dim': 'rgba(186,194,222,0.55)', '--oo-mute': 'rgba(147,153,178,0.55)',
      '--oo-canvas-dot': 'rgba(205,214,244,0.04)', '--oo-checker': '#1b1b28',
      '--oo-gridline': 'rgba(205,214,244,0.05)',
    },
  },
  {
    id: 'gruvbox',
    label: 'Gruvbox',
    mode: 'dark',
    vars: {
      '--oo-app': '#1d2021', '--oo-panel': '#282828', '--oo-chrome': '#232323',
      '--oo-popover': '#32302f', '--oo-input': '#3c3836',
      '--oo-hover': 'rgba(254,128,25,0.06)', '--oo-active': 'rgba(254,128,25,0.12)',
      '--oo-border': 'rgba(235,219,178,0.08)', '--oo-border2': 'rgba(235,219,178,0.18)',
      '--oo-accent': '#fe8019', '--oo-accent2': '#fabd2f', '--oo-onaccent': '#282828',
      '--oo-text': 'rgba(235,219,178,0.95)', '--oo-dim': 'rgba(213,196,161,0.50)', '--oo-mute': 'rgba(168,153,132,0.55)',
      '--oo-canvas-dot': 'rgba(235,219,178,0.04)', '--oo-checker': '#252422',
      '--oo-gridline': 'rgba(235,219,178,0.05)',
    },
  },
  {
    id: 'cyberpunk',
    label: 'Cyberpunk',
    mode: 'dark',
    vars: {
      '--oo-app': '#05050a', '--oo-panel': '#0a0a14', '--oo-chrome': '#07070f',
      '--oo-popover': '#101020', '--oo-input': '#181830',
      '--oo-hover': 'rgba(0,240,255,0.06)', '--oo-active': 'rgba(0,240,255,0.12)',
      '--oo-border': 'rgba(0,240,255,0.10)', '--oo-border2': 'rgba(0,240,255,0.25)',
      '--oo-accent': '#00f0ff', '--oo-accent2': '#ff2a6d', '--oo-onaccent': '#05050a',
      '--oo-text': 'rgba(224,255,255,0.95)', '--oo-dim': 'rgba(180,230,255,0.50)', '--oo-mute': 'rgba(120,180,220,0.35)',
      '--oo-canvas-dot': 'rgba(0,240,255,0.05)', '--oo-checker': '#0b0b16',
      '--oo-gridline': 'rgba(0,240,255,0.07)',
    },
  },
  {
    id: 'forest',
    label: 'Forest',
    mode: 'dark',
    vars: {
      '--oo-app': '#0b120c', '--oo-panel': '#111a12', '--oo-chrome': '#0d150e',
      '--oo-popover': '#16241a', '--oo-input': '#1d3022',
      '--oo-hover': 'rgba(74,222,128,0.06)', '--oo-active': 'rgba(74,222,128,0.12)',
      '--oo-border': 'rgba(134,239,172,0.08)', '--oo-border2': 'rgba(134,239,172,0.18)',
      '--oo-accent': '#4ade80', '--oo-accent2': '#a3e635', '--oo-onaccent': '#0b120c',
      '--oo-text': 'rgba(220,252,231,0.95)', '--oo-dim': 'rgba(187,247,208,0.45)', '--oo-mute': 'rgba(134,239,172,0.25)',
      '--oo-canvas-dot': 'rgba(134,239,172,0.05)', '--oo-checker': '#0f180f',
      '--oo-gridline': 'rgba(134,239,172,0.06)',
    },
  },
  {
    id: 'ocean',
    label: 'Ocean',
    mode: 'dark',
    vars: {
      '--oo-app': '#081019', '--oo-panel': '#0c1a26', '--oo-chrome': '#0a1420',
      '--oo-popover': '#112336', '--oo-input': '#163050',
      '--oo-hover': 'rgba(56,189,248,0.07)', '--oo-active': 'rgba(56,189,248,0.13)',
      '--oo-border': 'rgba(125,211,252,0.08)', '--oo-border2': 'rgba(125,211,252,0.20)',
      '--oo-accent': '#38bdf8', '--oo-accent2': '#22d3ee', '--oo-onaccent': '#081019',
      '--oo-text': 'rgba(224,242,254,0.95)', '--oo-dim': 'rgba(186,230,253,0.50)', '--oo-mute': 'rgba(125,211,252,0.30)',
      '--oo-canvas-dot': 'rgba(125,211,252,0.05)', '--oo-checker': '#0a1722',
      '--oo-gridline': 'rgba(125,211,252,0.06)',
    },
  },
  {
    id: 'rose',
    label: 'Rose',
    mode: 'dark',
    vars: {
      '--oo-app': '#170b10', '--oo-panel': '#1f0f16', '--oo-chrome': '#1a0d13',
      '--oo-popover': '#2a1520', '--oo-input': '#38192b',
      '--oo-hover': 'rgba(251,113,133,0.07)', '--oo-active': 'rgba(251,113,133,0.13)',
      '--oo-border': 'rgba(253,164,175,0.09)', '--oo-border2': 'rgba(253,164,175,0.20)',
      '--oo-accent': '#fb7185', '--oo-accent2': '#f0abfc', '--oo-onaccent': '#170b10',
      '--oo-text': 'rgba(255,228,230,0.95)', '--oo-dim': 'rgba(254,205,211,0.50)', '--oo-mute': 'rgba(253,164,175,0.30)',
      '--oo-canvas-dot': 'rgba(253,164,175,0.05)', '--oo-checker': '#1d1017',
      '--oo-gridline': 'rgba(253,164,175,0.06)',
    },
  },
  {
    id: 'ember',
    label: 'Ember',
    mode: 'dark',
    vars: {
      '--oo-app': '#120b08', '--oo-panel': '#1a110c', '--oo-chrome': '#150d0a',
      '--oo-popover': '#241611', '--oo-input': '#2f1d15',
      '--oo-hover': 'rgba(249,115,22,0.07)', '--oo-active': 'rgba(249,115,22,0.13)',
      '--oo-border': 'rgba(253,186,140,0.08)', '--oo-border2': 'rgba(253,186,140,0.18)',
      '--oo-accent': '#f97316', '--oo-accent2': '#fbbf24', '--oo-onaccent': '#120b08',
      '--oo-text': 'rgba(254,243,228,0.95)', '--oo-dim': 'rgba(253,230,208,0.50)', '--oo-mute': 'rgba(253,186,140,0.28)',
      '--oo-canvas-dot': 'rgba(253,186,140,0.05)', '--oo-checker': '#170e0a',
      '--oo-gridline': 'rgba(253,186,140,0.06)',
    },
  },
  {
    id: 'paper',
    label: 'Paper',
    mode: 'light',
    vars: {
      '--oo-app': '#e8e8ec', '--oo-panel': '#f5f5f7', '--oo-chrome': '#ededf2',
      '--oo-popover': '#ffffff', '--oo-input': '#e4e4e9',
      '--oo-hover': 'rgba(0,0,0,0.05)', '--oo-active': 'rgba(0,0,0,0.09)',
      '--oo-border': 'rgba(0,0,0,0.09)', '--oo-border2': 'rgba(0,0,0,0.18)',
      '--oo-accent': '#4f46e5', '--oo-accent2': '#7c3aed', '--oo-onaccent': '#ffffff',
      '--oo-text': 'rgba(15,15,20,0.92)', '--oo-dim': 'rgba(15,15,20,0.50)', '--oo-mute': 'rgba(15,15,20,0.30)',
      '--oo-canvas-dot': 'rgba(0,0,0,0.06)', '--oo-checker': '#dcdce2',
      '--oo-gridline': 'rgba(0,0,0,0.06)',
    },
  },
];

export const DEFAULT_THEME_ID = 'midnight';
const STORAGE_KEY = 'oo-theme';

export function getSavedThemeId(): string {
  try { return localStorage.getItem(STORAGE_KEY) || DEFAULT_THEME_ID; } catch { return DEFAULT_THEME_ID; }
}

export function getTheme(id: string): AppTheme {
  return THEMES.find(t => t.id === id) ?? THEMES[0];
}

/** Apply a theme by setting CSS vars + data attr on <html>. */
export function applyTheme(id: string) {
  const theme = getTheme(id);
  const root = document.documentElement;
  for (const [k, v] of Object.entries(theme.vars)) root.style.setProperty(k, v);
  root.dataset.theme = theme.id;
  root.style.colorScheme = theme.mode;
  try { localStorage.setItem(STORAGE_KEY, theme.id); } catch { /* noop */ }
}

/** Apply saved (or default) theme — call once at startup. */
export function initTheme(): string {
  const id = getSavedThemeId();
  applyTheme(id);
  return id;
}

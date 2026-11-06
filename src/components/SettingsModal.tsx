import React from 'react';
import { X, Palette, Keyboard } from 'lucide-react';
import { THEMES, getTheme } from '../themes';
import { cn } from '../utils';

export interface AppSettings {
  autoSave: boolean;
  snap: boolean;
  gridSize: number;
}

interface Props {
  themeId: string;
  onTheme: (id: string) => void;
  settings: AppSettings;
  onSettings: (s: AppSettings) => void;
  onShowShortcuts: () => void;
  onClose: () => void;
}

export default function SettingsModal({ themeId, onTheme, settings, onSettings, onShowShortcuts, onClose }: Props) {
  return (
    <div className="fixed inset-x-0 top-10 bottom-0 bg-black/70 backdrop-blur-sm z-50 flex items-center justify-center p-4" onClick={onClose}>
      <div className="bg-popover border border-line rounded-2xl w-full max-w-md overflow-hidden shadow-2xl flex flex-col max-h-[85vh]"
        onClick={e => e.stopPropagation()}>
        <div className="p-4 border-b border-line flex items-center justify-between shrink-0">
          <h2 className="font-bold text-oo flex items-center gap-2"><Palette size={16} className="text-accent" /> Settings & Themes</h2>
          <button onClick={onClose} className="text-dim hover:text-oo"><X size={16} /></button>
        </div>

        <div className="overflow-y-auto p-4 space-y-5">
          {/* Theme grid */}
          <div>
            <div className="text-[10px] uppercase font-semibold tracking-widest text-dim mb-2">Interface Theme</div>
            <div className="grid grid-cols-3 gap-2">
              {THEMES.map(t => (
                <button key={t.id} onClick={() => onTheme(t.id)}
                  className={cn("rounded-xl border p-2 text-left transition-all group",
                    themeId === t.id ? "border-accent bg-accent/10" : "border-line hover:border-line2")}>
                  {/* mini preview */}
                  <div className="h-10 rounded-lg mb-1.5 overflow-hidden flex border border-line"
                    style={{ background: t.vars['--oo-panel'] }}>
                    <div className="w-2/3 h-full" style={{ background: t.vars['--oo-app'] }}>
                      <div className="w-3/4 h-1.5 rounded-sm m-1.5" style={{ background: t.vars['--oo-accent'] }} />
                      <div className="w-1/2 h-1 rounded-sm mx-1.5" style={{ background: t.vars['--oo-accent2'], opacity: 0.7 }} />
                    </div>
                    <div className="flex-1 h-full" style={{ background: t.vars['--oo-panel'] }}>
                      <div className="w-1/2 h-1 rounded-sm m-1" style={{ background: t.vars['--oo-text'], opacity: 0.4 }} />
                    </div>
                  </div>
                  <div className="text-[10px] font-medium text-oo truncate">{t.label}</div>
                  <div className="text-[8px] text-mute uppercase tracking-wider">{t.mode}</div>
                </button>
              ))}
            </div>
          </div>

          {/* Editor settings */}
          <div>
            <div className="text-[10px] uppercase font-semibold tracking-widest text-dim mb-2">Editor</div>
            <div className="space-y-2">
              <label className="flex items-center justify-between py-1.5 cursor-pointer">
                <span className="text-sm text-oo">Auto-save workspace</span>
                <button onClick={() => onSettings({ ...settings, autoSave: !settings.autoSave })}
                  className={cn("w-9 h-5 rounded-full relative transition-colors", settings.autoSave ? "bg-accent" : "bg-active")}>
                  <div className={cn("absolute top-0.5 w-4 h-4 rounded-full bg-white shadow transition-all", settings.autoSave ? "left-[18px]" : "left-0.5")} />
                </button>
              </label>
              <label className="flex items-center justify-between py-1.5 cursor-pointer">
                <span className="text-sm text-oo">Snap to edges & center</span>
                <button onClick={() => onSettings({ ...settings, snap: !settings.snap })}
                  className={cn("w-9 h-5 rounded-full relative transition-colors", settings.snap ? "bg-accent" : "bg-active")}>
                  <div className={cn("absolute top-0.5 w-4 h-4 rounded-full bg-white shadow transition-all", settings.snap ? "left-[18px]" : "left-0.5")} />
                </button>
              </label>
              <div className="flex items-center justify-between py-1.5">
                <span className="text-sm text-oo">Grid size</span>
                <div className="flex gap-1">
                  {[10, 20, 50, 100].map(g => (
                    <button key={g} onClick={() => onSettings({ ...settings, gridSize: g })}
                      className={cn("px-2.5 py-1 rounded-md text-[11px] font-mono transition-colors",
                        settings.gridSize === g ? "bg-accent text-onaccent" : "bg-hover text-dim hover:bg-active")}>
                      {g}px
                    </button>
                  ))}
                </div>
              </div>
            </div>
          </div>

          <button onClick={onShowShortcuts}
            className="w-full py-2.5 rounded-xl bg-hover hover:bg-active text-sm font-medium text-oo flex items-center justify-center gap-2 transition-colors">
            <Keyboard size={15} /> Keyboard Shortcuts
          </button>
        </div>
      </div>
    </div>
  );
}

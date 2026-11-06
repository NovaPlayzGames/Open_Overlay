import React, { useEffect, useState } from 'react';
import { getCurrentWindow } from '@tauri-apps/api/window';
import { Minus, Square, X } from 'lucide-react';

export default function Titlebar() {
  const [isMaximized, setIsMaximized] = useState(false);
  const appWindow = getCurrentWindow();

  useEffect(() => {
    appWindow.isMaximized().then(setIsMaximized);
    const unlisten = appWindow.onResized(() => {
      appWindow.isMaximized().then(setIsMaximized);
    });
    return () => {
      unlisten.then(f => f());
    };
  }, [appWindow]);

  return (
    <div
      data-tauri-drag-region
      className="h-10 bg-chrome border-b border-line flex items-center justify-between select-none w-full shrink-0 relative z-[9999]"
    >
      <div className="flex-1 px-4 flex items-center pointer-events-none text-dim">
        <div className="w-4 h-4 rounded shadow-sm mr-3 flex items-center justify-center ring-1 ring-line2"
          style={{ background: 'linear-gradient(to bottom right, var(--oo-accent), var(--oo-accent2))' }} />
        <span className="text-xs font-bold tracking-[0.2em] text-dim uppercase">Open Overlay</span>
      </div>

      <div data-tauri-drag-region className="absolute inset-0 flex items-center justify-center pointer-events-none">
         <div className="text-[10px] font-medium text-mute tracking-widest uppercase bg-hover shadow-sm rounded-full px-4 py-1 border border-line pointer-events-none">
           Designer Environment
         </div>
      </div>

      <div className="flex items-center h-full relative z-10">
        <button
          className="h-full px-5 hover:bg-hover text-dim hover:text-oo transition-colors flex items-center justify-center"
          onClick={() => appWindow.minimize()}
        >
          <Minus size={14} />
        </button>
        <button
          className="h-full px-5 hover:bg-hover text-dim hover:text-oo transition-colors flex items-center justify-center"
          onClick={() => appWindow.toggleMaximize()}
        >
          <Square size={12} />
        </button>
        <button
          className="h-full px-5 hover:bg-red-500 hover:text-white text-dim transition-colors flex items-center justify-center"
          onClick={() => appWindow.close()}
        >
          <X size={14} />
        </button>
      </div>
    </div>
  );
}

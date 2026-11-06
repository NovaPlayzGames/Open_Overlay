import React from 'react';
import { getTheme } from '../themes';

interface Props {
  widgetName: string;
  width: number;
  height: number;
  zoom: number;
  elementCount: number;
  selectedLabel: string;
  dirty: boolean;
  autoSave: boolean;
  themeId: string;
}

export default function StatusBar({ widgetName, width, height, zoom, elementCount, selectedLabel, dirty, autoSave, themeId }: Props) {
  return (
    <div className="h-6 bg-chrome border-t border-line flex items-center px-4 gap-4 text-[10px] text-mute select-none shrink-0 z-20">
      <span className="font-medium text-dim truncate max-w-[140px]">{widgetName}</span>
      <span className="font-mono">{width}×{height}</span>
      <span className="w-px h-3 bg-line" />
      <span className="font-mono">{Math.round(zoom * 100)}%</span>
      <span className="w-px h-3 bg-line" />
      <span>{elementCount} element{elementCount === 1 ? '' : 's'}</span>
      <span className="w-px h-3 bg-line" />
      <span className="truncate flex-1">{selectedLabel}</span>
      {dirty && <span className="w-1.5 h-1.5 rounded-full bg-amber-400" title="Unsaved changes" />}
      <span>{autoSave ? 'autosave on' : 'autosave off'}</span>
      <span className="w-px h-3 bg-line" />
      <span className="flex items-center gap-1.5">
        <span className="w-2 h-2 rounded-full" style={{ background: getTheme(themeId).vars['--oo-accent'] }} />
        {getTheme(themeId).label}
      </span>
    </div>
  );
}

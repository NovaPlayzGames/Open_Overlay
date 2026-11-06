import React from 'react';
import { X } from 'lucide-react';

const SHORTCUTS: [string, string][] = [
  ['Ctrl+S', 'Save workspace'],
  ['Ctrl+Z / Ctrl+Y', 'Undo / Redo'],
  ['Ctrl+C / Ctrl+V', 'Copy / Paste element'],
  ['Ctrl+D', 'Duplicate element'],
  ['Ctrl+A', 'Select all elements'],
  ['Ctrl+G / Ctrl+Shift+G', 'Group / Ungroup selection'],
  ['Ctrl+] / Ctrl+[', 'Bring forward / Send backward'],
  ['Ctrl+= / Ctrl+- / Ctrl+0', 'Zoom in / out / fit'],
  ['Ctrl+/', 'Show shortcuts'],
  ['Delete / Backspace', 'Delete selection'],
  ['Arrow keys', 'Nudge 1px (Shift = 10px)'],
  ['Shift+Click', 'Multi-select layers'],
  ['Shift (rotate)', 'Snap rotation to 15°'],
  ['Shift (resize)', 'Lock aspect ratio'],
  ['V / C / P / E', 'Select / Pen / Pencil / Eraser tools'],
  ['K', 'Add keyframe (timeline open)'],
  ['Esc', 'Deselect / cancel tool'],
];

export default function ShortcutsModal({ onClose }: { onClose: () => void }) {
  return (
    <div className="fixed inset-x-0 top-10 bottom-0 bg-black/70 backdrop-blur-sm z-[60] flex items-center justify-center p-4" onClick={onClose}>
      <div className="bg-popover border border-line rounded-2xl w-full max-w-sm overflow-hidden shadow-2xl" onClick={e => e.stopPropagation()}>
        <div className="p-4 border-b border-line flex items-center justify-between">
          <h2 className="font-bold text-oo">Keyboard Shortcuts</h2>
          <button onClick={onClose} className="text-dim hover:text-oo"><X size={16} /></button>
        </div>
        <div className="p-3 max-h-[70vh] overflow-y-auto">
          {SHORTCUTS.map(([key, desc]) => (
            <div key={key} className="flex items-center justify-between py-1.5 px-2 rounded-lg hover:bg-hover">
              <span className="text-xs text-dim">{desc}</span>
              <kbd className="px-2 py-0.5 bg-active rounded-md text-[10px] font-mono text-oo border border-line shrink-0 ml-4">{key}</kbd>
            </div>
          ))}
        </div>
      </div>
    </div>
  );
}

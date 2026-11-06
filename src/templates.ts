import { v4 as uuidv4 } from 'uuid';
import { OverlayElement, Widget, WidgetType, WIDGET_PRESETS } from './types';

// ---------------------------------------------------------------------------
// Widget templates — pre-built element layouts for the template gallery.
// ---------------------------------------------------------------------------

let z = 0;
const el = (partial: Partial<OverlayElement>): OverlayElement => ({
  id: uuidv4(), type: 'shape', name: 'Element',
  x: 0, y: 0, width: 100, height: 100, zIndex: z++,
  visible: true, locked: false, opacity: 1, rotation: 0,
  ...partial,
});

export interface WidgetTemplate {
  id: string;
  label: string;
  description: string;
  widgetType: WidgetType;
  /** accent color shown in the gallery */
  swatch: string;
  build: () => Pick<Widget, 'elements' | 'background'>;
}

export const TEMPLATES: WidgetTemplate[] = [
  {
    id: 'alert-neon',
    label: 'Neon Alert',
    description: 'Glowing bordered alert with title + subtitle',
    widgetType: 'alert',
    swatch: '#00f0ff',
    build: () => ({
      background: 'transparent',
      elements: [
        el({ type: 'shape', name: 'Glow Frame', x: 0, y: 0, width: 600, height: 180,
          fill: 'rgba(5,5,15,0.85)', borderRadius: 18, strokeColor: '#00f0ff', strokeWidth: 2,
          shadowEnabled: true, shadowBlur: 24, shadowX: 0, shadowY: 0, shadowSpread: 0, shadowColor: 'rgba(0,240,255,0.45)' }),
        el({ type: 'shape', name: 'Accent Bar', x: 24, y: 36, width: 6, height: 108,
          fillMode: 'gradient', gradientFrom: '#00f0ff', gradientTo: '#ff2a6d', gradientAngle: 180, borderRadius: 3 }),
        el({ type: 'text', name: 'Title', x: 52, y: 40, width: 524, height: 56,
          content: '{event} ALERT', fontSize: 42, fontWeight: '800', color: '#ffffff',
          fontFamily: 'Inter', textAlign: 'left', letterSpacing: 2 }),
        el({ type: 'text', name: 'Subtitle', x: 52, y: 102, width: 524, height: 40,
          content: '{username} just followed!', fontSize: 26, fontWeight: '500',
          color: 'rgba(255,255,255,0.65)', fontFamily: 'Inter', textAlign: 'left',
          animationName: 'slideInLeft', animationDuration: 0.6, animationDelay: 0.2, animationIterationCount: '1' }),
      ],
    }),
  },
  {
    id: 'alert-minimal',
    label: 'Minimal Alert',
    description: 'Clean flat card with rounded corners',
    widgetType: 'alert',
    swatch: '#8b5cf6',
    build: () => ({
      background: 'transparent',
      elements: [
        el({ type: 'shape', name: 'Card', x: 0, y: 0, width: 600, height: 180,
          fill: '#18181b', borderRadius: 24,
          shadowEnabled: true, shadowBlur: 40, shadowX: 0, shadowY: 12, shadowSpread: -8, shadowColor: 'rgba(0,0,0,0.6)' }),
        el({ type: 'shape', name: 'Icon Disc', x: 28, y: 56, width: 68, height: 68,
          shapeType: 'circle', fillMode: 'gradient', gradientFrom: '#8b5cf6', gradientTo: '#3b82f6', gradientAngle: 135 }),
        el({ type: 'text', name: 'Title', x: 116, y: 48, width: 460, height: 44,
          content: 'New Follower', fontSize: 34, fontWeight: '700', color: '#ffffff', textAlign: 'left' }),
        el({ type: 'text', name: 'Subtitle', x: 116, y: 94, width: 460, height: 36,
          content: 'welcome to the stream', fontSize: 22, fontWeight: '400',
          color: 'rgba(255,255,255,0.5)', textAlign: 'left' }),
      ],
    }),
  },
  {
    id: 'chat-clean',
    label: 'Clean Chat',
    description: 'Rounded chat frame with gradient fade top',
    widgetType: 'chat',
    swatch: '#10b981',
    build: () => ({
      background: 'transparent',
      elements: [
        el({ type: 'shape', name: 'Chat BG', x: 0, y: 0, width: 400, height: 700,
          fill: 'rgba(10,10,12,0.75)', borderRadius: 20 }),
        el({ type: 'mask', name: 'Fade Top', x: 0, y: 0, width: 400, height: 700,
          maskType: 'gradient', gradientDir: 'to top', gradientStartOpacity: 1, gradientEndOpacity: 0,
          children: [] }),
        el({ type: 'text', name: 'Header', x: 24, y: 24, width: 352, height: 44,
          content: 'CHAT', fontSize: 24, fontWeight: '800', color: '#10b981',
          textAlign: 'left', letterSpacing: 6 }),
        el({ type: 'shape', name: 'Divider', x: 24, y: 72, width: 352, height: 2,
          fillMode: 'gradient', gradientFrom: '#10b981', gradientTo: 'transparent', gradientAngle: 90, borderRadius: 1 }),
      ],
    }),
  },
  {
    id: 'goal-gradient',
    label: 'Gradient Goal Bar',
    description: 'Progress bar with percentage label',
    widgetType: 'goal',
    swatch: '#3b82f6',
    build: () => ({
      background: 'transparent',
      elements: [
        el({ type: 'shape', name: 'Track', x: 0, y: 34, width: 550, height: 42,
          fill: 'rgba(255,255,255,0.08)', borderRadius: 21 }),
        el({ type: 'shape', name: 'Fill', x: 0, y: 34, width: 330, height: 42,
          fillMode: 'gradient', gradientFrom: '#3b82f6', gradientTo: '#8b5cf6', gradientAngle: 90,
          borderRadius: 21, animationName: 'pulse-slow', animationDuration: 3, animationIterationCount: 'infinite' }),
        el({ type: 'text', name: 'Label', x: 0, y: 0, width: 550, height: 28,
          content: 'SUB GOAL — 60%', fontSize: 20, fontWeight: '700', color: '#ffffff',
          textAlign: 'center', letterSpacing: 3 }),
      ],
    }),
  },
  {
    id: 'nowplaying-vinyl',
    label: 'Now Playing',
    description: 'Spinning disc + track text',
    widgetType: 'now_playing',
    swatch: '#ec4899',
    build: () => ({
      background: 'transparent',
      elements: [
        el({ type: 'shape', name: 'Card', x: 0, y: 0, width: 550, height: 130,
          fill: 'rgba(15,15,18,0.85)', borderRadius: 65 }),
        el({ type: 'shape', name: 'Disc', x: 20, y: 20, width: 90, height: 90,
          shapeType: 'circle', fillMode: 'gradient', gradientFrom: '#ec4899', gradientTo: '#8b5cf6', gradientAngle: 135,
          animationName: 'spin', animationDuration: 4, animationIterationCount: 'infinite' }),
        el({ type: 'shape', name: 'Disc Hole', x: 52, y: 52, width: 26, height: 26,
          shapeType: 'circle', fill: '#0f0f12' }),
        el({ type: 'text', name: 'Track', x: 130, y: 32, width: 400, height: 40,
          content: 'Song Title — Artist', fontSize: 26, fontWeight: '700', color: '#ffffff', textAlign: 'left' }),
        el({ type: 'text', name: 'Sub', x: 130, y: 74, width: 400, height: 30,
          content: 'NOW PLAYING', fontSize: 14, fontWeight: '600', color: 'rgba(255,255,255,0.4)',
          textAlign: 'left', letterSpacing: 4 }),
      ],
    }),
  },
  {
    id: 'clock-retro',
    label: 'Retro Clock',
    description: 'Outlined digits on dark pill',
    widgetType: 'clock',
    swatch: '#f59e0b',
    build: () => ({
      background: 'transparent',
      elements: [
        el({ type: 'shape', name: 'Pill', x: 0, y: 0, width: 320, height: 100,
          fill: 'rgba(0,0,0,0.7)', borderRadius: 50, strokeColor: '#f59e0b', strokeWidth: 2 }),
        el({ type: 'text', name: 'Time', x: 0, y: 18, width: 320, height: 64,
          content: '12:34 PM', fontSize: 44, fontWeight: '800', color: '#f59e0b',
          fontFamily: 'JetBrains Mono', textAlign: 'center', letterSpacing: 4 }),
      ],
    }),
  },
];

/** Instantiate a template into a full Widget. */
export function instantiateTemplate(t: WidgetTemplate, index: number): Widget {
  const preset = WIDGET_PRESETS[t.widgetType];
  const built = t.build();
  return {
    id: uuidv4(),
    name: t.label,
    widgetType: t.widgetType,
    width: preset.width,
    height: preset.height,
    background: built.background ?? 'transparent',
    artboardX: 60 + index * 40,
    artboardY: 60 + index * 40,
    elements: built.elements,
  };
}

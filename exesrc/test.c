//=========================================================================\
// test.c: tests the "edit color" dialog. As soon as the user click with   |
//         any mouse button on the client window, the "edit color" dialog  |
//         pops up and the client window background color is updated       |
//         accordingly with the selected color                             |
//=========================================================================/

#define SZ_CLIENTCLASS         "testclient"
#define SZ_TESTTITLE           "\"Edit color\" test"
#define TID_TITLE              100

#pragma strings(readonly)

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <stdio.h>
#include "editcol.h"

MRESULT EXPENTRY ClientWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

int main(void) {
   ULONG flFrame = FCF_STANDARD & ~(FCF_ACCELTABLE | FCF_MENU);
   HWND hFrame, hClient;
   QMSG qmsg;
   HAB hab = WinInitialize(0);
   HMQ hmq = WinCreateMsgQueue(hab, 0);

   WinRegisterClass(hab, SZ_CLIENTCLASS, ClientWndProc, CS_SIZEREDRAW, 0) ;                // Extra bytes to reserve
   if (NULLHANDLE != (hFrame = WinCreateStdWindow(HWND_DESKTOP, WS_VISIBLE,   
                                                  &flFrame, SZ_CLIENTCLASS,
                                                  SZ_TESTTITLE, 0L, NULLHANDLE,
                                                  1, &hClient))) {
      while (WinGetMsg(hab, &qmsg, NULLHANDLE, 0, 0))
         WinDispatchMsg(hab, &qmsg);
   } // end if
   WinDestroyWindow(hFrame);
   WinDestroyMsgQueue(hmq);
   WinTerminate(hab);
   return 0;
}

MRESULT EXPENTRY ClientWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   static CLR clr;
   CLR clrRet;
   HPS hps;
   RECTL rcl;
   char achTxt[64];
 
   switch (msg) {
      case WM_CREATE:
         clr.rgb.red = 204;
         clr.rgb.grn = 204;
         clr.rgb.blu = 204;
         break;
      case WM_COLORWHEELCHANGED:
      case WM_COLORWHEELCHANGED3:
         clr.lClr = (LONG)mp1;
         WinInvalidateRect(hwnd, NULL, FALSE);
         sprintf(achTxt, "Color (RGB): %d - % d - %d",
                 clr.rgb.red, clr.rgb.grn, clr.rgb.blu);
         WinSetWindowText(WinQueryWindow(hwnd, QW_PARENT), achTxt);
         break;
      case WM_BUTTON1CLICK:
      case WM_BUTTON2CLICK:
      case WM_BUTTON3CLICK:
         clr.lClr = WinEditColorDlg(HWND_DESKTOP, hwnd,
                                    (COLOR)clr.lClr, NULL);
         WinInvalidateRect(hwnd, NULL, FALSE);
         sprintf(achTxt, "Returned color (RGB): %d - % d - %d",
                 clr.rgb.red, clr.rgb.grn, clr.rgb.blu);
         WinSetWindowText(WinQueryWindow(hwnd, QW_PARENT), achTxt);
         WinStartTimer(WinQueryAnchorBlock(hwnd), hwnd, TID_TITLE, 5000);
         return (MRESULT)TRUE;
      case WM_TIMER:
         if ((USHORT)mp1 == TID_TITLE)
            WinSetWindowText(WinQueryWindow(hwnd, QW_PARENT), SZ_TESTTITLE);
            WinStopTimer(WinQueryAnchorBlock(hwnd), hwnd, TID_TITLE);
         break;
      case WM_PAINT:
         hps = WinBeginPaint(hwnd, NULLHANDLE, &rcl);
         GpiCreateLogColorTable(hps, 0, LCOLF_RGB, 0, 0, NULL);
         WinFillRect(hps, &rcl, clr.lClr);
         WinEndPaint(hps);
         break;
      default: 
         return WinDefWindowProc(hwnd, msg, mp1, mp2);
   } // end switch
   return (MRESULT)FALSE;
}

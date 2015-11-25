/*	File:		ExampleVideoPanel.c	Contains:	Example video panel component routines.	Written by:	Gary Woodcock	Copyright:	� 1992 by Apple Computer, Inc., all rights reserved.	Change History (most recent first):*///-----------------------------------------------------------------------// includes#include "ExampleVideoPanelPrivate.h"#include <QuickTimeComponents.h>#include <Errors.h>#include <Resources.h>#include <SysEqu.h>//-----------------------------------------------------------------------#ifdef DEBUG_IT// Use this declaration when we're running linked (for debugging)pascal ComponentResultExampleVideoPanelDispatcher (ComponentParameters *params, Handle storage)											 #else// Use this declaration when we're a standalone componentpascal ComponentResultmain (ComponentParameters *params, Handle storage)#endif DEBUG_IT{	// This routine is the main dispatcher for the component		ComponentResult		result = noErr;	ComponentFunction	exampleFunction = nil;		// Did we get a Component Manager request code (< 0)?	if (params->what < 0)	{		switch (params->what)		{			case kComponentOpenSelect:			// Open request			{				exampleFunction = _ExampleVideoPanelOpen;				break;			}			case kComponentCloseSelect:			// Close request			{				exampleFunction = _ExampleVideoPanelClose;				break;			}			case kComponentCanDoSelect:			// Can Do request			{				result = CallComponentFunction (params, 					(ComponentFunction) _ExampleVideoPanelCanDo);				break;			}			case kComponentVersionSelect:		// Version request			{				result = CallComponentFunction (params,					(ComponentFunction) _ExampleVideoPanelVersion);				break;			}			case kComponentTargetSelect:		// Target request not supported			case kComponentRegisterSelect:		// Register request not supported			default:							// Unknown request			{				result = badComponentSelector;				break;			}		}	}	else	// Was it one of our request codes?	{		switch (params->what)		{			case kSGCPanelGetDitlSelect:		// SGPanelGetDitl request			{				exampleFunction = _ExampleVideoPanelGetDitl;				break;			}			case kSGCPanelGetTitleSelect:		// SGPanelGetTitle request			{				exampleFunction = _ExampleVideoPanelGetTitle;				break;			}			case kSGCPanelCanRunSelect:			// SGPanelCanRun request			{				exampleFunction = _ExampleVideoPanelCanRun;				break;			}			case kSGCPanelInstallSelect:		// SGPanelInstall request			{				exampleFunction = _ExampleVideoPanelInstall;				break;			}			case kSGCPanelEventSelect:			// SGPanelEvent request			{				exampleFunction = _ExampleVideoPanelEvent;				break;			}			case kSGCPanelItemSelect:			// SGPanelItem request			{				exampleFunction = _ExampleVideoPanelItem;				break;			}			case kSGCPanelRemoveSelect:			// SGPanelRemove request			{				exampleFunction = _ExampleVideoPanelRemove;				break;			}			case kSGCPanelSetGrabberSelect:		// SGPanelSetGrabber request			{				exampleFunction = _ExampleVideoPanelSetGrabber;				break;			}			case kSGCPanelSetResFileSelect:		// SGPanelSetResFile request			{				exampleFunction = _ExampleVideoPanelSetResFile;				break;			}			case kSGCPanelGetSettingsSelect:	// SGPanelGetSettings request			{				exampleFunction = _ExampleVideoPanelGetSettings;				break;			}			case kSGCPanelSetSettingsSelect:	// SGPanelSetSettings request			{				exampleFunction = _ExampleVideoPanelSetSettings;				break;			}			default:							// Unknown request			{				result = badComponentSelector;				break;			}		}	}	if (exampleFunction != nil)	{		result = CallComponentFunctionWithStorage (storage, params, exampleFunction);	}	return (result);}											 //-----------------------------------------------------------------------pascal ComponentResult_ExampleVideoPanelOpen (Handle storage, ComponentInstance self){	#pragma	unused (storage)		PanelGlobalsHdl	globals;	ComponentResult	result = noErr;		#ifdef THINK_C		SetComponentInstanceA5 (self, *(long *) CurrentA5);	#endif THINK_C		// Can we open another instance?	if (CountComponentInstances ((Component) self) <= kMaxExampleVideoPanelInstances)	{		// Did we get our storage?		globals = (PanelGlobalsHdl) NewHandleClear (sizeof (PanelGlobals));		if (globals != nil)		{			// Keep a reference to self			(**globals).self = (Component) self;									// Set storage ref			SetComponentInstanceStorage (self, (Handle) globals);		}		else	// NewHandleClear failed		{			result = MemError();		}	}	else	// No more instances can be opened	{		result = kGenericError;	}	return (result);}//-----------------------------------------------------------------------pascal ComponentResult_ExampleVideoPanelClose (Handle storage, ComponentInstance self){	PanelGlobalsHdl	globals = (PanelGlobalsHdl) storage;	ComponentResult	result = noErr;		// Do we have any clean up to do?	if (globals != nil)	{		// Dispose globals		DisposHandle ((Handle) globals);	}	return (result);}//-----------------------------------------------------------------------pascal ComponentResult_ExampleVideoPanelCanDo (short selector){	// Case on the request code	switch (selector)	{		// Component Manager request codes		case kComponentOpenSelect:		case kComponentCloseSelect:		case kComponentCanDoSelect:		case kComponentVersionSelect:				// Sequence grabber panel component request codes		case kSGCPanelGetDitlSelect:			case kSGCPanelGetTitleSelect:			case kSGCPanelCanRunSelect:				case kSGCPanelInstallSelect:			case kSGCPanelEventSelect:				case kSGCPanelItemSelect:				case kSGCPanelRemoveSelect:				case kSGCPanelSetGrabberSelect:		case kSGCPanelSetResFileSelect:		case kSGCPanelGetSettingsSelect:		case kSGCPanelSetSettingsSelect:		{			return (true);		}				// Unsupported or unknown request codes		case kComponentRegisterSelect:	// Register request not supported		case kComponentTargetSelect:	// Target request not supported		default:						// Not a request code we recognize		{			return (false); 		}	}}//-----------------------------------------------------------------------pascal ComponentResult_ExampleVideoPanelVersion (void){	// Return the version info	return (exampleVideoPanelInterfaceRevision);}//-----------------------------------------------------------------------pascal ComponentResult_ExampleVideoPanelGetDitl (Handle storage, Handle *ditl){	// This routine gets our ditl and hands it back to the sequence grabber		#pragma unused (storage)	ComponentResult	result = noErr;	Handle			panelDITL = GetResource ('DITL', kExampleVideoPanelDITLID);	// Did we get the DITL resource okay?	result = ResError();		if (panelDITL != nil)	{		// Detach it		DetachResource (panelDITL);	}	*ditl = panelDITL;		return (result);}//-----------------------------------------------------------------------pascal ComponentResult_ExampleVideoPanelGetTitle (Handle storage, Str255 title){	#pragma unused (storage)	#pragma unused (title)		// Nothing to do here right now	return (paramErr);}//-----------------------------------------------------------------------pascal ComponentResult_ExampleVideoPanelCanRun (Handle storage, SGChannel c){	#pragma unused (storage)	#pragma unused (c)	// Our 'thng' resource has the channelFlagHasDependency flag set, 	// which means that this routine gets called to find out if	// it can run in the current environment.  This usually means	// finding out if the panel is compatible with the currently	// selected digitizer.  This is handy for adding manufacturer	// specific panels to the settings dialog.  We don't do anything	// here except SysBeep just to show we actually get called.	SysBeep(5);		return (noErr);}//-----------------------------------------------------------------------pascal ComponentResult_ExampleVideoPanelInstall (Handle storage, SGChannel c, DialogPtr d,	short itemOffset){	// Do our setup in this routine		#pragma unused (d)	#pragma unused (itemOffset)		PanelGlobalsHdl	globals = (PanelGlobalsHdl) storage;	ComponentResult	result = noErr;	// Get our gray pattern	*(long *)((**globals).grayPat) = *(long *)((**globals).grayPat + 4) = 0x55AA55AA;		// Get current black level	result = VDGetBlackLevelValue (SGGetVideoDigitizerComponent (c), &((**globals).savedBlackLevel));	if (result != noErr)	{		// Can't get the black level, so we can't do zero black level		HiliteControl ((ControlHandle) GetItemHandle (d, itemOffset + kZeroBlackButton), 255);	}	return (noErr);}//-----------------------------------------------------------------------pascal ComponentResult_ExampleVideoPanelEvent (Handle storage, SGChannel c, DialogPtr d,	short itemOffset, EventRecord *theEvent, short *itemHit, Boolean *handled){	// This routine is quite similar to a normal event filter proc.		PanelGlobalsHdl	globals = (PanelGlobalsHdl) storage;	ComponentResult	result = noErr;	GrafPtr			savedPort;	PenState		savedPen;		// Set up the port stuff	GetPort (&savedPort);	SetPort (d);	GetPenState (&savedPen);							// Assume we don't handle it	*handled = false;		// Look for a key hit	if ((theEvent->what == keyDown) || (theEvent->what == autoKey))	{		char	theChar = theEvent->message & charCodeMask;		char	theKeyCode = ((theEvent->message & keyCodeMask) >> 8);		Boolean	cmdKeyDown = ((theEvent->modifiers & cmdKey) != 0) ? true : false;				if (cmdKeyDown && (theKeyCode == kDKey))		{			unsigned short	maxBlackLevel = 0;						// Fake a "Do it" button hit			FakeDialogButtonHit (d, itemOffset + kZeroBlackButton);			result = VDSetBlackLevelValue (SGGetVideoDigitizerComponent (c), &maxBlackLevel);			*itemHit = itemOffset + kZeroBlackButton;			*handled = true;		}		else if (cmdKeyDown && (theKeyCode == kRKey))		{			// Fake a "Reset" button hit			FakeDialogButtonHit (d, itemOffset + kResetButton);			result = VDSetBlackLevelValue (SGGetVideoDigitizerComponent (c), &((**globals).savedBlackLevel));			*itemHit = itemOffset + kResetButton;			*handled = true;		}	}	else if (theEvent->what == updateEvt)	{		Rect	r;				// Draw the separator line whenever we get an update cuz I'm lazy		GetItemBox (d, itemOffset + kSeparator, &r);		PenPat ((**globals).grayPat);		FrameRect (&r);	}		// Restore stuff	SetPenState (&savedPen);	SetPort (savedPort);		return (result);}//-----------------------------------------------------------------------pascal ComponentResult_ExampleVideoPanelItem (Handle storage, SGChannel c, DialogPtr d,	short itemOffset, short itemNum){	#pragma unused (d)		PanelGlobalsHdl	globals = (PanelGlobalsHdl) storage;	ComponentResult	result = noErr;	short			theItem = itemNum - itemOffset;	// Remember to account for the item offset		// What item got hit?	switch (theItem)	{		case kZeroBlackButton:		{			unsigned short	maxBlackLevel = 0;						result = VDSetBlackLevelValue (SGGetVideoDigitizerComponent (c), &maxBlackLevel);			break;		}		case kResetButton:		{			result = VDSetBlackLevelValue (SGGetVideoDigitizerComponent (c), &((**globals).savedBlackLevel));			break;		}		default:		{			break;		}	}	return (result);}//-----------------------------------------------------------------------pascal ComponentResult_ExampleVideoPanelRemove (Handle storage, SGChannel c, DialogPtr d,	short itemOffset){	#pragma unused (storage)	#pragma unused (c)	#pragma unused (d)	#pragma unused (itemOffset)		// This is where you do your panel related cleanup.  Note that	// this is different from the cleanup you do in your close component	// routine.  Basically, this gets called when your ditl is getting	// removed from the dialog and a new one is being added (like when	// someone chooses a different panel from the panel popup menu).	// Examples of stuff you might do here include getting rid of any	// custom controls or popup menus, or saving panel settings.		return (noErr);}//-----------------------------------------------------------------------pascal ComponentResult_ExampleVideoPanelSetGrabber (Handle storage, SeqGrabComponent sg){	PanelGlobalsHdl	globals = (PanelGlobalsHdl) storage;		// Save our grabber	(**globals).seqGrabber = sg;		return (noErr);}//-----------------------------------------------------------------------pascal ComponentResult_ExampleVideoPanelSetResFile (Handle storage, short resRef){	PanelGlobalsHdl	globals = (PanelGlobalsHdl) storage;		// Since we don't have the channelFlagDontOpenResFile flag	// set in our 'thng' resource, the sequence grabber will 	// open our resource file for us.		// Save our resfile ref	(**globals).resRefNum = resRef;		return (noErr);}														 //-----------------------------------------------------------------------pascal ComponentResult_ExampleVideoPanelGetSettings (Handle storage, SGChannel c, UserData *ud, long flags){	#pragma unused (c)	#pragma unused (flags)		ComponentResult	result = noErr;	UserData		userStuff = 0L;		// You HAVE to give something valid back as user data when you	// return from this routine or your panel will fail to install.  	// What is being asked for basically amounts to a handle to some 	// state info specific to your panel.  We don't really have any 	// state info we care about, so we just hand back an empty user 	// data just to keep everyone happy.		if ((result = NewUserData (&userStuff)) == noErr)	{		*ud = userStuff;	}	return (result);}														//-----------------------------------------------------------------------pascal ComponentResult_ExampleVideoPanelSetSettings (Handle storage, SGChannel c, UserData ud, long flags){	#pragma unused (storage)	#pragma unused (c)	#pragma unused (ud)	#pragma unused (flags)		// Here is where you would decode your user data structure and use	// the info to set the state of your panel items.  Again, we don't	// really have anything that has any meaningful state in our panel,	// so we ignore this.		return (noErr);}														 //-----------------------------------------------------------------------pascal ComponentResult_ExampleVideoPanelValidateInput (Handle storage, Boolean *ok){	#pragma unused (storage)		// This is where you do a sanity check on the user-definable	// items in your panel.  If there are any bad values, you	// should alert the user somehow, and return false for *ok.	// If everything's fine, return true.  We don't really have	// anything to check on in our example, so we return true	// all the time.		*ok = true;	return (noErr);}//-----------------------------------------------------------------------static OSErrFakeButtonHit (ControlHandle theButton){	OSErr	result = noErr;		if (theButton != nil)	{		long	dummyTicks = 0L;				HiliteControl (theButton, inButton);		Delay (8, &dummyTicks);		HiliteControl (theButton, 0);	}	else	{		result = nilHandleErr;	}	return (result);}//-----------------------------------------------------------------------static OSErrFakeDialogButtonHit (DialogPtr theDialog, short theButtonItem){	OSErr	result = noErr;		if (theDialog != nil)	{		result = FakeButtonHit ((ControlHandle) GetItemHandle (theDialog, theButtonItem));	}	else	{		result = nilHandleErr;	}	return (result);}//-----------------------------------------------------------------------static voidSetUserItem (DialogPtr theDialog, short theItem, Handle userProc){	Handle	item;	Rect	box;	short	itemType;		GetDItem (theDialog, theItem, &itemType, &item, &box);	SetDItem (theDialog, theItem, itemType, userProc, &box);}//-----------------------------------------------------------------------static HandleGetItemHandle (DialogPtr theDialog, short theItem){	Handle	item;	Rect	box;	short	itemType;		GetDItem (theDialog, theItem, &itemType, &item, &box);	return (item);}//-----------------------------------------------------------------------static voidGetItemBox (DialogPtr theDialog, short theItem, Rect *theRect){	Handle	item;	short	itemType;		GetDItem (theDialog, theItem, &itemType, &item, theRect);}//-----------------------------------------------------------------------#ifdef THINK_C#ifdef DEBUG_ITvoidRegisterExampleVideoPanel (void);voidRegisterExampleVideoPanel (void){	ComponentDescription foo;	Handle h;  	foo.componentType = SeqGrabPanelType;  	foo.componentSubType = 'vide';  	foo.componentManufacturer = 'xmpl';  	foo.componentFlags = channelFlagHasDependency | channelFlagDontOpenResFile;	// Indicate that we want a can run message  	foo.componentFlagsMask = 0L;	PtrToHand ("\pExample (linked)", &h, 17);	RegisterComponent (&foo, (void *)ExampleVideoPanelDispatcher, 0, h, 0, 0);	DisposHandle (h);}#endif DEBUG_IT#endif THINK_C//-----------------------------------------------------------------------
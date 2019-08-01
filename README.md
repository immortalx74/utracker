uTracker is a tiny, tracker-like music authoring application. It's not meant as an alternative or replacement of any of the fine trackers out there (OpenMPT, ScreamTracker ImpulseTracker, etc.). It's simply a little coding project, which just mimics the very basic functionality and workflow of what is considered to be a tracker.

#### For Beginners: What's a music tracker?

It's a music authoring tool which is based on playing-back sound samples at specified intervals. It's visualized in a grid (called a **pattern**), with rows representing the smallest time interval, and columns (called **tracks** or **channels**) which are used to "stack" several samples together. Several patterns can be created to make a complete song (or in tracker-terms, a **module**). Unlike other music authoring tools, the patterns are arranged and scrolled vertically.

The most basic functionality of a tracker, is the ability to playback a sample at a different frequency, thus producing different musical notes. That means that the resulting file size is orders of magnitude smaller than recorded audio, and was the main reason of the widespread use of trackers through the 80's and 90's.

Although nowadays the drop in prices of audio hardware with pro features, in combination with affordable (or even free) DAW software is the preferred method of producing music at home, hardcore enthusiasts and pros alike still use trackers for music production, as it's the most immediate way to be creative.

#### Quick Guide:
The uTracker window is split in 3 panels:

 -   **Left Panel**
    
     - ***Patterns list***
Use the patterns list buttons to add, remove, move up/down patterns, and access the current pattern's options. Clicking on an entry in the list will set it as the active pattern. At the options window you can change the current pattern's name, and add/delete rows. 
```Note: If there's existing data on deleted rows, it too gets deleted.```
 
     - ***Instruments list***
Use the instruments list buttons to add, remove, move up/down instruments, and access the current instrument's options. Clicking on an entry in the list will set it as the active instrument. At the options window you can change the current instrument's name and edit its sample-map.

		The sample-map is a matrix where rows correspond to loaded samples and columns to musical notes.
		By default, a newly created instrument will have all its notes assigned to sample #0. There's an exception to this which depends on the way samples are imported (explained bellow at the Samples list section).
		At any one time, you can see the mapped samples of a single octave. Drag the "octave" slider to select a different octave. Clicking on a cell assigns a note of the current octave to a sample.
		At the bottom of the options window, the buttons labeled ***Current octave***, ***All octaves*** can be used to quickly set the notes of the current octave or the notes of all octaves, respectively , to the highlighted sample.
		```Note:The most usual case for an instrument is to have all of its notes assigned to a single sample (an analogy to real-life instruments). However, in the case of percussion sounds for example, it's useful to have many adjacent notes mapped to different sounds, as a shortcut for easy note entry (e.g. alternating between kick, snare and hi-hat). ```

     - ***Samples list***
Use the samples list buttons to add and remove samples. The icon labelled "+2" is used to add an instrument/sample pair.
This is a quick way to load a sample and automatically create an instrument with all its notes mapped to this sample. This also auto-assigns the sample's filename to the instrument name for convenience.
```Note: If a sample that was previously mapped to one or more notes of any instrument gets deleted, these specific notes of the affected instrument(s) will get reset to sample #0. If sample #0 gets deleted too, any such notes entered in a pattern will simply be ignored```

     - ***Control sliders***
The "Middle octave" slider, is used to set the octave of the notes triggered by the middle row of the keyboard.
So if the slider is set to octave 5, the entered notes will have their octave as bellow:
(Q, W, E, ...) -> octave 4
(A, S, D, ...) -> octave  5 (middle octave)
(Z, X, C, ...) -> octave 6

		The "BPM" slider sets the module's Beats Per Minute.

		The ""Rows/Beat" slider sets the module's Rows Per Beat.
		
		The "Step" slider sets the number of rows the cursor will "jump" down when a new note is entered. This is used to quickly enter drum beats for example.

		The "Master volume" slider controls the module's overall volume.
-   Toolbar
    - New
    - Open
    - Save
    - Save as
    - Play module
    - Play pattern
    - Pause
    - Stop
    - Toggle repeat
    - Cut
    - Copy
    - Paste
    - Open settings
-   Pattern Editor

	At the top of the pattern editor you'll find the track headers. At the very left of the track headers, the buttons labelled "+", "-" are used to add and remove tracks. When you add a track you'll be asked to select an existing track, and a position (before/after) to add it to.
To delete a track, just select an existing track from the list in the pop-up.

    -	The track headers have the following controls:

		Solo - Mute buttons. When you toggle the solo of a track all other tracks are muted. The mute button just mutes that specific track. To quickly reset all tracks to the default  un-muted state, toggle twice any track's solo button.
	
		Volume - Pan sliders. These control each individual track's volume and pan respectively. Right click any slider to reset it to default (64 for volume, 0 for pan).

- The editor grid
	This is the heart of the tracker! Left click on a cell to set it as active, or use the arrow keys to navigate.
	Click and drag to make a selection. You can either right-click inside the grid to access the clipboard options or by using the corresponding toolbar buttons.
	```Note: To navigate the grid it first has to be "in-focus". This is indicated by an outline on the edit cursor. You can either click inside the grid or press the Enter key to set it in-focus.```
	Each track is divided in 5 cells per row which correspond to: Note, Instrument, Volume, FX and FX parameters. In this version of uTracker, FX and FX parameters are not yet implemented so they are combined in a single "dummy" cell for simplicity.
	
	Explaining everything having to do with entering data to a tracker is beyond the scope of this guide, so just expected it to be just like any tracker out there!

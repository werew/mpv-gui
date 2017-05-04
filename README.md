# mpv-gui
A QT GUI for mpv: https://mpv.io/

Command for launching mpv in idle mode with IPC socket:
`mpv --idle --input-ipc-server=/tmp/mpvsocket`


# Setting 

- Play: 
- Pause: {"commnad": ["set_property", "pause", true]}
- Stop: {"command": ["stop"]}
- Next (playlist): {"command": ["playlist-next"]}
- Prev (playlist): {"command": ["playlist-prev"]}
- Fast forward  { "command": ["seek", 0.2, "relative"] } or { "command": ["set_property", "speed", 4.0] }
- Fast backward { "command": ["seek", -0.2, "relative"] }
- Volume {"command": ["set_property", "volume", "50"]}
- Progress bar { "command": ["set_property", "percent-pos", "100"] }
- Piece selection (radio, list, files):
        {"command": ["loadfile","filename"]}
        {"command": ["loadlist","listname"]}


# Observing 

- Volume {"command": ["observe_property", 1, "volume"]}
- Progress bar: { "command": ["observe_property", 2, "percent-pos"] }
- Filename: {"command": ["observe_property", 3, "filename"]}
- Pause: {"command": ["observe_property", 4, "pause"]}
- Stop: { "command": ["observe_property", 5, "idle-active"] }



# TODO list

- Complete selection playlist (DONE)
- I18N (DONE)
- Buttons next and previous (DONE)


- Verify QT 5....
- Pretty description
- Size window
- Report
- Execution from zero
- Names on files
- Test on config (missing keys)





At the time of writing, Allegro 5 seems to have issues on
some systems with the default sound driver, which appears to
be PulseAudio. This will cause CBEnchanted to simply freeze
upon launch, with an empty window open.

To fix this issue, create `/etc/allegro5rc` with the
following content (Or edit it if it already exists)
    [audio]
    driver=alsa #or oss or openal, depending on what you use. Alsa should work.
If you cannot do this due to, for example, lack of root
privileges, try creating `~/allegro5rc` (The documentation
erroneously refers to it as `~/.allegro5rc` which at the
time of writing was *not* accepted by Allegro.) with the
same contents.

This should fix the issue. Apologies for the inconvenience
but the issue is not caused by CBEnchanted code and thus
cannot be fixed by the developers!

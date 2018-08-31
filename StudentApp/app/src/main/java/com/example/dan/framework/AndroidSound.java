package com.example.dan.framework;

import android.app.Activity;
import android.media.MediaPlayer;
import android.media.SoundPool;
import uk.ac.leonarduea.haig.studentexperience.View.R;
//import com.example.dan.framework.Sound;



public class AndroidSound extends Activity implements Sound {
    int soundId = R.raw.correct;
    SoundPool soundPool;
    /**
     *contructor to create the objects
     * @param soundPool
     * @param soundId
     */
    public AndroidSound() {
        this.soundId = soundId;
        this.soundPool = soundPool;
    }

    /**
     * play the sound and set the volume
     * @param volume
     */
    @Override
    public void play(float volume) {
        //soundPool.play(soundId, volume, volume, 0, 0, 1);
        final MediaPlayer correct = MediaPlayer.create(this, soundId);
        correct.start();
    }

    /**
     *  dispose of the sound
     *  using the sound id
     */
    @Override
    public void dispose() {
        soundPool.unload(soundId);
    }

    /**
     * to stop the sound playing
     * using the sound id
     */
    @Override
    public void stop() {
        soundPool.stop(soundId);
    }

}

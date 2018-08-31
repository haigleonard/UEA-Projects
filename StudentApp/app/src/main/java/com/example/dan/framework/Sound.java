package com.example.dan.framework;

public interface Sound {
    /**
     * play the sound and set the volume
     * @param volume
     */
    public void play(float volume);
    /**
     *  dispose of the sound
     *  using the sound id
     */
    public void dispose();

    /**
     * to stop the sound playing
     * using the sound id
     */
    public void stop();

}

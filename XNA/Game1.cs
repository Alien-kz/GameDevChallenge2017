using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;
using Microsoft.Xna.Framework.Net;
using Microsoft.Xna.Framework.Storage;

namespace NewPong
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        Texture2D ball_texture, left_bat_texture, right_bat_texture;
        Ball ball;
        Bat left_bat, right_bat;
        SpriteFont spritefont;
        bool bubbles = true;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
        }

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            // TODO: Add your initialization logic here
            ball = new Ball(this.Window.ClientBounds.Width, this.Window.ClientBounds.Height);
            left_bat = new Bat(this.Window.ClientBounds.Width, this.Window.ClientBounds.Height, true);
            right_bat = new Bat(this.Window.ClientBounds.Width, this.Window.ClientBounds.Height, false);
            ball.left_bat = left_bat;
            ball.right_bat = right_bat;
            //left_bat.ball = ball;
            //right_bat.ball = ball;
            base.Initialize();
        }

        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);
            ball_texture = Content.Load<Texture2D>("ball_wide");
            left_bat_texture = Content.Load<Texture2D>("left_bat_wide");
            right_bat_texture = Content.Load<Texture2D>("right_bat_wide");
            spritefont = Content.Load<SpriteFont>("SpriteFont1");
            // TODO: use this.Content to load your game content here
        }

        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// all content.
        /// </summary>
        protected override void UnloadContent()
        {
            // TODO: Unload any non ContentManager content here
        }

        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            // TODO: Add your update logic here
            left_bat.Update(gameTime);
            right_bat.Update(gameTime);
            ball.Update(gameTime);
            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);
            // TODO: Add your drawing code here
            spriteBatch.Begin();
            spriteBatch.Draw(ball_texture, ball.position, new Rectangle(0, 0, Ball.width, Ball.height), Color.White);
            spriteBatch.Draw(left_bat_texture, left_bat.position, new Rectangle(0, 0, left_bat.width, left_bat.height), Color.White);
            spriteBatch.Draw(right_bat_texture, right_bat.position, new Rectangle(0, 0, right_bat.width, right_bat.height), Color.White);
            //spriteBatch.DrawString(spritefont, ball.speed.Length().ToString(), new Vector2(0, 0), Color.White);
            //spriteBatch.DrawString(spritefont, left_bat.position.Y.ToString(), new Vector2(0, 0), Color.White);
            if (bubbles)
            {
                spriteBatch.DrawString(spritefont, left_bat.score.ToString(), new Vector2(left_bat.position.X + left_bat.width / 2, left_bat.bubble), Color.Black);
                spriteBatch.DrawString(spritefont, right_bat.score.ToString(), new Vector2(right_bat.position.X + right_bat.width / 2, right_bat.bubble), Color.Black);
            }
            else
            {
                spriteBatch.DrawString(spritefont, left_bat.score.ToString(), new Vector2(0, 0), Color.Black);
                spriteBatch.DrawString(spritefont, right_bat.score.ToString(), new Vector2(right_bat.position.X + right_bat.width, 0), Color.Black);
            }
            spriteBatch.End();
            base.Draw(gameTime);
        }
    }
}

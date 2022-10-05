#include <simplecpp>
#include "shooter.h"
#include "bubble.h"

/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);

double mod(double p)
{ if(p >= 0){return p;}
  else {return -p;}}

void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

void break_bubble(vector<Bubble> &bubbles, vector<Bullet> &bullets, Color color){
     // break a bigger bubble to form two smaller bubbles
for (unsigned int b=0; b < bubbles.size(); b++)
        { for (unsigned int k=0; k < bullets.size(); k++)
            {if(((bullets[k].r_get_center_x()-bubbles[b].c_get_center_x())*(bullets[k].r_get_center_x()-bubbles[b].c_get_center_x()) +
                (bullets[k].r_get_center_y()-bubbles[b].c_get_center_y())*(bullets[k].r_get_center_y()-bubbles[b].c_get_center_y()) <=
                (bullets[k].r_get_width()+bubbles[b].c_get_radius())*(bullets[k].r_get_width()+bubbles[b].c_get_radius()) +
                (bullets[k].r_get_height()+bubbles[b].c_get_radius())*(bullets[k].r_get_height()+bubbles[b].c_get_radius()))
                && (bubbles[b].c_get_radius() != BUBBLE_RADIUS_THRESHOLD))
                { bubbles.push_back(Bubble(bubbles[b].c_get_center_x(), bubbles[b].c_get_center_y(), bubbles[b].c_get_radius()/2,
                    -BUBBLE_DEFAULT_VX, bubbles[b].c_get_vy(), color));
                  bubbles.push_back(Bubble(bubbles[b].c_get_center_x(), bubbles[b].c_get_center_y(), bubbles[b].c_get_radius()/2,
                    BUBBLE_DEFAULT_VX, bubbles[b].c_get_vy(), color));
                  bullets.erase(bullets.begin()+k);
                  bubbles.erase(bubbles.begin()+b);
                }
            }
        }
}


// delete bubbles and the bullets if the bullets hit the bubbles of threshold radius
void erase_bubble_and_bullet(unsigned int n, vector<Bubble> &bubbles, vector<Bullet> &bullets)
{   for (unsigned int b=0; b < n; b++)
        { for (unsigned int k=0; k < bullets.size(); k++)
            {if(((bullets[k].r_get_center_x()-bubbles[b].c_get_center_x())*(bullets[k].r_get_center_x()-bubbles[b].c_get_center_x()) +
                (bullets[k].r_get_center_y()-bubbles[b].c_get_center_y())*(bullets[k].r_get_center_y()-bubbles[b].c_get_center_y()) <=
                (bullets[k].r_get_width()+bubbles[b].c_get_radius())*(bullets[k].r_get_width()+bubbles[b].c_get_radius()) +
                (bullets[k].r_get_height()+bubbles[b].c_get_radius())*(bullets[k].r_get_height()+bubbles[b].c_get_radius()))
                && (bubbles[b].c_get_radius() == BUBBLE_RADIUS_THRESHOLD))
                { bullets.erase(bullets.begin()+k);
                  bubbles.erase(bubbles.begin()+b);}
            }
        }
}

vector<Bubble> create_bubbles_lvl_1()
{
    // create initial bubbles in the game in level 1
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, COLOR(255,20,147)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(255,20,147)));
    return bubbles;
}

vector<Bubble> create_bubbles_lvl_2()
{
    // create initial bubbles in the game in level 2
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, COLOR(255,140,0)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(255,140,0)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, COLOR(255,140,0)));
    return bubbles;
}

vector<Bubble> create_bubbles_lvl_3()
{
    // create initial bubbles in the game in level 3
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, COLOR(0,191,255)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(0,191,255)));
    bubbles.push_back(Bubble(3*WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, COLOR(0,191,255)));
    bubbles.push_back(Bubble(WINDOW_X, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(0,191,255)));
    return bubbles;
}

int main()
{   bool shooter_bubble_collision = 0;

    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

    //LEVEL 1
{   Text lvl_1 (250,250,"LEVEL 1");
    lvl_1.scale(400);
    wait(2);
    lvl_1.move(10000,0);

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles_lvl_1();

    XEvent event;

    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }

        // Update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets);

        // Check for bigger bubble and bullet collision
        break_bubble(bubbles, bullets, COLOR(128,0,128));

        // Check for bigger bubble and bullet collision
        erase_bubble_and_bullet(bubbles.size(), bubbles, bullets);

        if ( bubbles.size() == 0 ){break;} // Check for end of level

        //Bubble shooter collision
       for(unsigned int B=0; B < bubbles.size(); B++){
        if ((mod(shooter.get_body_center_x()-bubbles[B].c_get_center_x())<=bubbles[B].c_get_radius()+shooter.get_body_width()/2)
         &&
         (mod(shooter.get_body_center_y()-bubbles[B].c_get_center_y())<=bubbles[B].c_get_radius()+2*shooter.get_head_radius()+shooter.get_body_height()/2))
               { shooter_bubble_collision=1;}}

        if( shooter_bubble_collision == 1){wait(2); break;} // Check for Game Over

        wait(STEP_TIME);
    }

}

if( shooter_bubble_collision == 0)
{   //LEVEL 2
    Text lvl_2 (250,250,"LEVEL 2");
    lvl_2.scale(400);
    wait(2);
    lvl_2.move(10000,0);

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles_lvl_2();

    XEvent event;

    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }

        // Update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets);

        // Check for bigger bubble and bullet collision
        break_bubble(bubbles, bullets, COLOR(255,215,0));

        // Check for bigger bubble and bullet collision
        erase_bubble_and_bullet(bubbles.size(), bubbles, bullets);

        if ( bubbles.size() == 0 ){break;}  // Check for end of level

       //Bubble shooter collision
       for(unsigned int B=0; B < bubbles.size(); B++){
        if ((mod(shooter.get_body_center_x()-bubbles[B].c_get_center_x())<=bubbles[B].c_get_radius()+shooter.get_body_width()/2)
         &&
         (mod(shooter.get_body_center_y()-bubbles[B].c_get_center_y())<=bubbles[B].c_get_radius()+2*shooter.get_head_radius()+shooter.get_body_height()/2))
               { shooter_bubble_collision=1;}}

        if( shooter_bubble_collision == 1){wait(2); break;} // Check for Game Over

        wait(STEP_TIME);
    }

}

if( shooter_bubble_collision == 0)
{   //LEVEL 3
    Text lvl_3 (250,250,"LEVEL 3");
    lvl_3.scale(400);
    wait(2);
    lvl_3.move(10000,0);

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles_lvl_3();

    XEvent event;

    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }

        // Update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets);

        // Check for bigger bubble and bullet collision
        break_bubble(bubbles, bullets, COLOR(0,0,128));

        // Check for bigger bubble and bullet collision
        erase_bubble_and_bullet(bubbles.size(), bubbles, bullets);

        if ( bubbles.size() == 0 ){break;}  // Check for end of level

       //Bubble shooter collision
       for(unsigned int B=0; B < bubbles.size(); B++){
        if ((mod(shooter.get_body_center_x()-bubbles[B].c_get_center_x())<=bubbles[B].c_get_radius()+shooter.get_body_width()/2)
         &&
         (mod(shooter.get_body_center_y()-bubbles[B].c_get_center_y())<=bubbles[B].c_get_radius()+2*shooter.get_head_radius()+shooter.get_body_height()/2))
               { shooter_bubble_collision=1;}}

        if( shooter_bubble_collision == 1){wait(2); break;} // Check for Game Over

        wait(STEP_TIME);
    }

}
if( shooter_bubble_collision == 1)
{Text t(250, 200, "Game Over!!!!" );
t.scale(400);
wait(5);
t.move(10000,0);}
else
{Text T(250, 200, "You have completed the game." );
T.scale(400);
wait(5);
T.move(10000,0);}
}

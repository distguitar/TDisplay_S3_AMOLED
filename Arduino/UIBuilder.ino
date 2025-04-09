// Indicator Colors
uint16_t inactiveColor = gfx->color565(2,36,162);
uint16_t activeColor = gfx->color565(11,176,3);
uint16_t lowColor = gfx->color565(11,176,3);
uint16_t medColor = gfx->color565(252,205,19);
uint16_t highColor = gfx->color565(160,36,8);

int cpuTicks[20] = {100,90,80,70,60,50,40,30,20,10,0,10,5,20,50,30,20,60,50,40};

/////////////////////////////////////////////////////////////////
// Build CPU Ticks UI
/////////////////////////////////////////////////////////////////
void BuildCPUUI()
{
  gfx->fillRect(6,6,6+524,6+60,gfx->color565(2,36,162));

  int startX = 6;
  int startY = 6;
  int barW = 26;
  int barH = 60;
  for (int c=0;c<20;c++)
  {
    int val = cpuTicks[c];
    int barSize = (val * 60) / 100;
    int x = startX + (barW * c);
    int y = startY + (barH - barSize);
    int w = barW;
    int h = barSize;

    gfx->fillRect(x,y,w,h,gfx->color565(0,53,252));
  }

  // Output CPU percentage
  gfx->setTextColor(WHITE);
  gfx->setTextSize(4 /* x scale */, 4 /* y scale */, 1 /* pixel_margin */);
  gfx->setCursor(222, 20);
  int cpu = cpuTicks[19];
  gfx->print(cpu);
  gfx->print("%");
}

/////////////////////////////////////////////////////////////////
// Update latest CPU Tick Value
// Value in % - 0 to 100
/////////////////////////////////////////////////////////////////
void UpdateCPUTick(int val)
{
  if (val >=100)
  {
    val = 100;
  }

  // Shift array to the left
  for (int i=1;i<20;i++)
  {
    cpuTicks[i-1] = cpuTicks[i];
  }
  cpuTicks[19] = val;
}

/////////////////////////////////////////////////////////////////
// Build RAM UI
// Value in % - 0 to 100
/////////////////////////////////////////////////////////////////
void BuildRAMUI(int val)
{
  if (val >= 100)
  {
    val = 100;
  }
  BuildBarUI(val,6);
  gfx->setTextColor(WHITE);
  gfx->setTextSize(2 /* x scale */, 2 /* y scale */, 1 /* pixel_margin */);
  
  // Clear previous text
  gfx->fillRect(80,140,150,25,gfx->color565(0,0,0));
  gfx->setCursor(80, 140);
  gfx->print("RAM: ");
  gfx->print(val);
  gfx->print("%");
}

/////////////////////////////////////////////////////////////////
// Build LAN UI
// Value in % - 0 to 100
/////////////////////////////////////////////////////////////////
void BuildLANUI(int val)
{
  if (val >= 100)
  {
    val = 100;
  }
  BuildBarUI(val,280);
  gfx->setTextColor(WHITE);
  gfx->setTextSize(2 /* x scale */, 2 /* y scale */, 1 /* pixel_margin */);

  // Clear previous text
  gfx->fillRect(350,140,150,25,gfx->color565(0,0,0));
  gfx->setCursor(350, 140);
  gfx->print("LAN: ");
  gfx->print(val);
  gfx->print("%");
}

/////////////////////////////////////////////////////////////////
// Build Bar UI (Used by RAM and LAN)
// Value in % - 0 to 100
/////////////////////////////////////////////////////////////////
void BuildBarUI(int val, int startX)
{
  val = val / 10;
  int startY = 90;
  int barH = 40;
  int barW = 22;
  int barSpace = 2;

  for (int i=0;i<10;i++)
  {
    int x = startX + ((barW + barSpace) * i);
    int y = startY;
    int h = barH;
    int w = barW;

    if (i<val)
    {
      gfx->fillRect(x,y,w,h,activeColor);
    }
    else
    {
      gfx->fillRect(x,y,w,h,gfx->color565(0,0,0));
      gfx->drawRect(x,y,w,h,inactiveColor);
    }
  }
}

/////////////////////////////////////////////////////////////////
// Update Drive Space Bars
// Value in % - 0 to 100
// drive: 0 - C:, 1 - D:, 2 - F:, 3 - G:
/////////////////////////////////////////////////////////////////
void BuildDriveUI(int val, int drive)
{
  if (val >= 100)
  {
    val = 100;
  }

  val = val / 10;

  int startX;
  int startY;
  int barH = 25;
  int barW = 240;
  switch (drive)
  {
    case 0: // C drive
      startX = 30;
      startY = 175;
    break;
    case 1: // D drive
      startX = 30;
      startY = 208;
    break;
    case 2: // F drive
      startX = 270;
      startY = 175;
    break;
    case 3: // G drive
      startX = 270;
      startY = 208;
    break;
  }

  // Clear background
  gfx->fillRect(startX,startY,barW,barH,gfx->color565(0,0,0));

  // Draw background bar
  gfx->drawRect(startX,startY,barW,barH,gfx->color565(2,36,162));

  uint16_t color;
  if (val <= 3)
  {
    color = lowColor;
  }
  else if (val > 3 && val <6)
  {
    color = medColor;
  }
  else
  {
    color = highColor;
  }

  int indicatorWidth = (val * (barW / 10));
  gfx->fillRect(startX,startY,indicatorWidth,barH,color);
}

/////////////////////////////////////////////////////////////////
// Build Drive Labels
/////////////////////////////////////////////////////////////////
void BuildTextLabels()
{
  // Drive Letters
  gfx->setTextColor(WHITE);
  gfx->setTextSize(3 /* x scale */, 3 /* y scale */, 1 /* pixel_margin */);
  gfx->setCursor(2, 178);
  gfx->println("C");
  gfx->setCursor(2, 208);
  gfx->println("D");
  gfx->setCursor(512, 178);
  gfx->println("F");
  gfx->setCursor(512, 208);
  gfx->println("G");
}

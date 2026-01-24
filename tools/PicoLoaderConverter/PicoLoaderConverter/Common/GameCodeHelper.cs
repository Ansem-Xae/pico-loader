namespace PicoLoaderConverter.Common;

static class GameCodeHelper
{
    public static uint GameCodeToUint(string gameCode)
    {
        if (gameCode.Length != 4)
        {
            throw new ArgumentException(
                $"Game code '{gameCode}' is not valid. It must consist of exactly 4 characters.", nameof(gameCode));
        }
        return gameCode[0] | (uint)gameCode[1] << 8 | (uint)gameCode[2] << 16 | (uint)gameCode[3] << 24;
    }
}

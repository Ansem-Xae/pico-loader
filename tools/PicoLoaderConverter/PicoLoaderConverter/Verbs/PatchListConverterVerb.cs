using CommandLine;
using PicoLoaderConverter.PatchList;

namespace PicoLoaderConverter.Verbs;

[Verb("patchlist", HelpText = "Convert patch list from json to bin.")]
sealed class PatchListConverterVerb : IConverterVerb
{
    [Option('i', Required = true, HelpText = "Input .json file.")]
    public required string InputFile { get; init; }

    [Option('o', Required = true, HelpText = "Output .bin file.")]
    public required string OutputFile { get; init; }

    public void Run()
    {
        // Convert patch list from json to bin
        var factory = new PatchListFactory();
        var patchList = factory.FromJson(File.ReadAllText(InputFile));
        var binaryList = factory.ToBinary(patchList);
        File.WriteAllBytes(OutputFile, binaryList);
    }
}

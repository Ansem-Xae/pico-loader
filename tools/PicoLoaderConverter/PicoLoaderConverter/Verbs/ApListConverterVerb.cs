using CommandLine;
using PicoLoaderConverter.ApList;

namespace PicoLoaderConverter.Verbs;

[Verb("aplist", HelpText = "Convert ap list from csv to bin.")]
sealed class ApListConverterVerb : IConverterVerb
{
    [Option('i', Required = true, HelpText = "Input .csv file.")]
    public required string InputFile { get; init; }

    [Option('o', Required = true, HelpText = "Output .bin file.")]
    public required string OutputFile { get; init; }

    public void Run()
    {
        // Convert ap list from csv to bin
        var factory = new ApListFactory();
        var apList = factory.FromCsv(File.ReadAllText(InputFile));
        var binaryList = factory.ToBinary(apList);
        File.WriteAllBytes(OutputFile, binaryList);
    }
}
